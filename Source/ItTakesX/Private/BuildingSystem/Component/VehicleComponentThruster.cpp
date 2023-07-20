// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/VehicleComponentThruster.h"

#include "Components/ArrowComponent.h"

void AVehicleComponentThruster::BeginPlay()
{
	Super::BeginPlay();

	TArray<USceneComponent*> AllComponents;
	RootComponent->GetChildrenComponents(true, AllComponents);
	for (auto Comp : AllComponents)
	{
		if (Comp->ComponentTags.Contains(TEXT("ForwardArrow")))
		{
			BP_ForwardArrowRef = Cast<UArrowComponent>(Comp);
		}
	}
}

void AVehicleComponentThruster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRunning)
	{
		if (BP_ForwardArrowRef)
		{
			Mesh->AddForce(BP_ForwardArrowRef->GetForwardVector() * CurrenForceLength, NAME_None, true);
		}
		else
		{
			Mesh->AddForce(FVector::UpVector * CurrenForceLength, NAME_None, true);
		}
	}
}
