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

FQuat UVehicleConnectionComponent::GetDirectionRotation(UVehicleConnectionComponent* OtherConnectionComponent)
{
	if (OtherConnectionComponent == nullptr || ConnectionDirection == nullptr || OtherConnectionComponent->
		ConnectionDirection == nullptr)
	{
		return FQuat::Identity;
	}

	auto OutAxis = FVector::CrossProduct(ConnectionDirection->GetForwardVector(),
	                                     OtherConnectionComponent->ConnectionDirection->GetForwardVector());
	auto OutAngle = 180 - FMath::RadiansToDegrees(
		FMath::Acos(FVector::DotProduct(ConnectionDirection->GetForwardVector(),
		                                OtherConnectionComponent->ConnectionDirection->GetForwardVector())));
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("OutAxis = %s, OutDegree = %f"), *OutAxis.ToString(), OutAngle));
	OutAxis.Normalize();
	return FQuat(OutAxis, FMath::DegreesToRadians(OutAngle));
}

FQuat UVehicleConnectionComponent::GetAlignmentRotation(UVehicleConnectionComponent* OtherConnectionComponent)
{
	if (OtherConnectionComponent == nullptr || ConnectionDirection == nullptr)
	{
		return FQuat::Identity;
	}

	if (ConnectionAlignments.Num() < 1)
	{
		return FQuat::Identity;
	}

	auto FirstArrow = ConnectionAlignments[0];
	FVector Axis = FVector::UpVector;
	float MinAngle = 180;
	for (auto OtherArrow : OtherConnectionComponent->ConnectionAlignments)
	{
		float CurrenAngle = FMath::RadiansToDegrees(
			FMath::Acos(FVector::DotProduct(FirstArrow->GetForwardVector(), OtherArrow->GetForwardVector())));
		if (CurrenAngle < MinAngle)
		{
			Axis = FVector::CrossProduct(OtherArrow->GetForwardVector(), FirstArrow->GetForwardVector());
			MinAngle = CurrenAngle;
		}
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("OutAxis = %s, OutDegree = %f, ConnectionAlignments num: %d "),
	// 	                                 *ConnectionDirection->GetForwardVector().ToString(), MinAngle,
	// 	                                 ConnectionAlignments.Num()));

	if (MinAngle >= 180)
	{
		return FQuat::Identity;
	}

	Axis.Normalize();

	return FQuat(Axis, FMath::DegreesToRadians(MinAngle));
}
