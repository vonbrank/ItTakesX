// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/HealthComponent.h"

#include "Ability/DrivingComponent.h"
#include "BuildingSystem/VehicleControllerActor.h"
#include "Character/ItTakesXCharacter.h"
#include "Enemy/EnemyBasePawn.h"
#include "Environment/DeadZone.h"
#include "GameModes/ItTakesXGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &ThisClass::DamageTaken);
	DrivingComponent = GetOwner()->FindComponentByClass<UDrivingComponent>();

	ItTakesXGameMode = Cast<AItTakesXGameMode>(UGameplayStatics::GetGameMode(this));
}

void UHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                   AController* DamageInstigator, AActor* DamageCauser)
{
	if (Cast<IVehicleNode>(DamageCauser) || DamageInstigator == GetWorld()->GetFirstPlayerController())
	{
		return;
	}

	auto CurrentVehicleController =
		DrivingComponent != nullptr ? DrivingComponent->GetCurrentDrivingVehicle() : nullptr;

	if (CurrentVehicleController)
	{
		if (Cast<ADeadZone>(DamageCauser))
		{
			Damage *= 10;
		}

		CurrentVehicleController->DamageTaken(DamagedActor, Damage, DamageType, DamageInstigator, DamageCauser);
		return;
	}

	if (Health <= 0)
	{
		return;
	}

	Health -= Damage;

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Character Health = %f"), Health));

	if (Health <= 0 && ItTakesXGameMode)
	{
		ItTakesXGameMode->CharacterDied(Cast<AItTakesXCharacter>(GetOwner()));
	}
	// if (DamageCauser)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	// 	                                 FString::Printf(TEXT("DamagedActor %s"), *DamageCauser->GetName()));
	// }
}

float UHealthComponent::GetHealthPercentage()
{
	return FMath::Max(0, Health) / MaxHealth;
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
