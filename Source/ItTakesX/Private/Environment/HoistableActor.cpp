// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/HoistableActor.h"

// Sets default values
AHoistableActor::AHoistableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHoistableActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHoistableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHoistableActor::OnBeginAiming_Implementation(AActor* OtherActor)
{
	CurrentAimingActor = OtherActor;
}

void AHoistableActor::OnEndAiming_Implementation(AActor* OtherActor)
{
	CurrentAimingActor = nullptr;
}

void AHoistableActor::OnBeginHoisting_Implementation(AActor* OtherActor)
{
	CurrentHoistingActor = OtherActor;

	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(RootComponent);
	if (PrimitiveComponent)
	{
		PrimitiveComponent->SetSimulatePhysics(false);
	}
}

void AHoistableActor::OnEndHoisting_Implementation(AActor* OtherActor)
{
	CurrentHoistingActor = nullptr;

	UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(RootComponent);
	if (PrimitiveComponent)
	{
		PrimitiveComponent->SetSimulatePhysics(true);
	}
}

bool AHoistableActor::IsHoisting() const
{
	return CurrentHoistingActor != nullptr;
}
