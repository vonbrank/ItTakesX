// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/VehicleComponentFlameThrower.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"

AVehicleComponentFlameThrower::AVehicleComponentFlameThrower()
{
	FireParticleSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FireParticleSpawnPoint"));
	FireParticleSpawnPoint->SetupAttachment(RootComponent);

	FireArea = CreateDefaultSubobject<UBoxComponent>(TEXT("FireArea"));
	FireArea->SetupAttachment(RootComponent);
}

void AVehicleComponentFlameThrower::BeginPlay()
{
	Super::BeginPlay();

	// ToggleOpenFire();
}

void AVehicleComponentFlameThrower::ToggleOpenFire()
{
	if (CurrentFireParticle)
	{
		CurrentFireParticle->Destroy();
		CurrentFireParticle = nullptr;
	}
	else
	{
		CurrentFireParticle = GetWorld()->SpawnActor<AEmitter>(FireParticleClass,
		                                                       FireParticleSpawnPoint->GetComponentLocation(),
		                                                       FireParticleSpawnPoint->GetComponentRotation());
		CurrentFireParticle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AVehicleComponentFlameThrower::SetIsRunning(bool bNewIsRunning)
{
	Super::SetIsRunning(bNewIsRunning);

	if (!bNewIsRunning)
	{
		if (CurrentFireParticle)
		{
			CurrentFireParticle->Destroy();
			CurrentFireParticle = nullptr;
		}
	}
}
