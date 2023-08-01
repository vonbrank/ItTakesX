// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/VehicleConnectionComponent.h"

#include "Components/ArrowComponent.h"

// Sets default values for this component's properties
UVehicleConnectionComponent::UVehicleConnectionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UVehicleConnectionComponent::BeginPlay()
{
	Super::BeginPlay();

	{
		TArray<USceneComponent*> ChildComponents;
		GetChildrenComponents(false, ChildComponents);

		for (auto Component : ChildComponents)
		{
			UArrowComponent* ArrowComponent = Cast<UArrowComponent>(Component);
			if (ArrowComponent)
			{
				ConnectionDirection = ArrowComponent;
				break;
			}
		}
	}

	if (ConnectionDirection)
	{
		TArray<USceneComponent*> ChildComponents;
		ConnectionDirection->GetChildrenComponents(false, ChildComponents);
		for (auto Component : ChildComponents)
		{
			UArrowComponent* ArrowComponent = Cast<UArrowComponent>(Component);
			if (ArrowComponent)
			{
				ConnectionAlignments.Add(ArrowComponent);
			}
		}
	}
}


// Called every frame
void UVehicleConnectionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                                FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FVector UVehicleConnectionComponent::GetDirectionArrowLocation()
{
	if (ConnectionDirection)
	{
		return ConnectionDirection->GetComponentLocation();
	}
	return GetComponentLocation();
}
