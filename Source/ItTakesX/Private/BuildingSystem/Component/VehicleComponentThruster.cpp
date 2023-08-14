// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/VehicleComponentThruster.h"

#include "NiagaraComponent.h"
#include "Components/ArrowComponent.h"

AVehicleComponentThruster::AVehicleComponentThruster()
{
	ForwardArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ForwardArrow"));
	ForwardArrow->SetupAttachment(RootComponent);

	BladeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BladeMesh"));
	BladeMesh->SetupAttachment(RootComponent);

	WindBarParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WindBarParticle"));
	WindBarParticle->SetupAttachment(RootComponent);
}

void AVehicleComponentThruster::BeginPlay()
{
	Super::BeginPlay();
}

void AVehicleComponentThruster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRunning)
	{
		Mesh->AddForce(ForwardArrow->GetForwardVector() * CurrentThrottle, NAME_None, true);


		auto BladeDeltaRotation = FQuat(BladeMesh->GetForwardVector(),
		                                MaxThrottle * DeltaTime * CurrentThrottle / MaxThrottle / 100);

		BladeMesh->SetWorldRotation(BladeDeltaRotation * BladeMesh->GetComponentRotation().Quaternion());
	}
}

void AVehicleComponentThruster::Throttle(float Value)
{
	CurrentThrottle = Value;
	// if (CurrentThrottle < 0)
	// {
	// 	CurrentThrottle = 0;
	// }
	// else if (CurrentThrottle > MaxThrottle)
	// {
	// 	CurrentThrottle = MaxThrottle;
	// }
	// CurrentThrottle = FMath::Clamp(0, MaxThrottle, CurrentThrottle);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(
		                                 TEXT("throttling percentage: %.f%%"),
		                                 CurrentThrottle / MaxThrottle * 100));
}

void AVehicleComponentThruster::SetIsRunning(bool bNewIsRunning)
{
	Super::SetIsRunning(bNewIsRunning);

	if (!bNewIsRunning)
	{
		CurrentThrottle = 0;
		WindBarParticle->SetVisibility(false);
	}
	else
	{
		WindBarParticle->SetVisibility(true);
	}
}
