// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/DeadZone.h"

#include "Character/ItTakesXCharacter.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ADeadZone::ADeadZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaBox"));
	SetRootComponent(AreaBox);
}

// Called when the game starts or when spawned
void ADeadZone::BeginPlay()
{
	Super::BeginPlay();

	AreaBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxStartOverlap);
	AreaBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnBoxEndOverlap);
}

void ADeadZone::OnBoxStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	auto NewCharacter = Cast<AItTakesXCharacter>(OtherActor);
	if (NewCharacter)
	{
		CurrentCharacter = NewCharacter;
		if (CurrentCharacter->GetMesh()->GetPhysicsLinearVelocity().Length() > CharacterMaxSpeed)
		{
			auto NewSpeed = CurrentCharacter->GetCharacterMovement()->Velocity;
			NewSpeed.Normalize();
			NewSpeed *= CharacterMaxSpeed;
			CurrentCharacter->GetMesh()->SetPhysicsLinearVelocity(NewSpeed);
		}
	}
}

void ADeadZone::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == CurrentCharacter)
	{
		CurrentCharacter = nullptr;
	}
}

// Called every frame
void ADeadZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrentCharacter)
	{
		auto DamageType = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(CurrentCharacter, DamageSpeed * DeltaTime, nullptr, this, DamageType);
	}
}
