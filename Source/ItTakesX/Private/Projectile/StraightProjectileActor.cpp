// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/StraightProjectileActor.h"

#include "BuildingSystem/VehicleControllerActor.h"
#include "Character/ItTakesXCharacter.h"
#include "Enemy/EnemyBasePawn.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AStraightProjectileActor::AStraightProjectileActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStraightProjectileActor::BeginPlay()
{
	Super::BeginPlay();
}

void AStraightProjectileActor::DamageTarget(UPrimitiveComponent* HitComp, AActor* OtherActor,
                                            UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                                            const FHitResult& HitResult)
{
	auto ThisOwner = GetOwner();
	AController* ThisInstigator = nullptr;

	auto EnemyPawn = Cast<AEnemyBasePawn>(ThisOwner);
	if (EnemyPawn)
	{
		ThisInstigator = EnemyPawn->GetController();
	}
	else
	{
		ThisInstigator = UGameplayStatics::GetPlayerController(this, 0);
	}

	auto DamageType = UDamageType::StaticClass();
	UGameplayStatics::ApplyDamage(OtherActor, 20, ThisInstigator, this, DamageType);

	auto Character = Cast<AItTakesXCharacter>(OtherActor);
	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(OtherComp);

	if (Character)
	{
		auto VehicleController = Character->GetVehicleControllerFromCharacter();
		if (VehicleController)
		{
			VehicleController->GetRootMesh()->AddImpulse(-HitResult.Normal * HitImpulse, NAME_None, true);
		}
		else
		{
			Character->GetCharacterMovement()->AddImpulse(-HitResult.Normal * HitImpulse, true);
		}
	}
	else if (PrimitiveComponent && OtherActor->IsRootComponentMovable())
	{
		PrimitiveComponent->AddImpulse(-HitResult.Normal * HitImpulse, NAME_None, true);
	}
}

// Called every frame
void AStraightProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
