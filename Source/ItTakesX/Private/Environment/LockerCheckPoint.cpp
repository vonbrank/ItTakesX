// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/LockerCheckPoint.h"

#include "BuildingSystem/VehicleComponentActor.h"
#include "Components/SphereComponent.h"

ALockerCheckPoint::ALockerCheckPoint()
{
	ButtonArea = CreateDefaultSubobject<USphereComponent>(TEXT("ButtonArea"));
	ButtonArea->SetupAttachment(RootComponent);
}

void ALockerCheckPoint::BeginPlay()
{
	Super::BeginPlay();

	ButtonArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::ALockerCheckPoint::OnButtonSphereStartOverlap);
	ButtonArea->OnComponentEndOverlap.AddDynamic(
		this, &ThisClass::ALockerCheckPoint::ALockerCheckPoint::OnButtonSphereEndOverlap);
}

bool ALockerCheckPoint::CanSpawnTokenCube()
{
	return CurrentOverlappingButtonActor != nullptr && Super::CanSpawnTokenCube();
}

void ALockerCheckPoint::OnButtonSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult)
{
	auto VehicleComponentActor = Cast<AVehicleComponentActor>(OtherActor);

	if (VehicleComponentActor && VehicleComponentActor->Tags.Contains(NeedCubeTag))
	{
		CurrentOverlappingButtonActor = VehicleComponentActor;
	}
}

void ALockerCheckPoint::OnButtonSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == CurrentOverlappingButtonActor)
	{
		CurrentOverlappingButtonActor = nullptr;
	}
}
