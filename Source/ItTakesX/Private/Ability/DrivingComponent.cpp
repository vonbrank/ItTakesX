// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/DrivingComponent.h"

#include "BuildingSystem/Core/VehicleCoreHoveringCar.h"

// Sets default values for this component's properties
UDrivingComponent::UDrivingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDrivingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UDrivingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDrivingComponent::SetCurrenOverlappingVehicle(AVehicleCoreActor* NewOverlappingVehicle)
{
	CurrentOverlappingVehicle = NewOverlappingVehicle;
}

// bool UDrivingComponent::ExecuteVehicleCommand(FVehicleCoreCommand Command)
// {
// 	if (CurrentOverlappingVehicle == nullptr)
// 	{
// 		return false;
// 	}
//
// 	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Start executing command")));
//
// 	return CurrentOverlappingVehicle->PropagateCommand(Command);
// }

bool UDrivingComponent::InteractWithMoveForward(float Value)
{
	if (CurrentOverlappingVehicle == nullptr)
	{
		return false;
	}

	if (!CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	AVehicleCoreHoveringCar* VehicleCoreHoveringCar = Cast<AVehicleCoreHoveringCar>(CurrentOverlappingVehicle);
	if (VehicleCoreHoveringCar)
	{
		VehicleCoreHoveringCar->HandleControl(EHoveringCarControl_MoveForward, Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::InteractWithTurnRight(float Value)
{
	if (CurrentOverlappingVehicle == nullptr)
	{
		return false;
	}
	if (!CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	AVehicleCoreHoveringCar* VehicleCoreHoveringCar = Cast<AVehicleCoreHoveringCar>(CurrentOverlappingVehicle);
	if (VehicleCoreHoveringCar)
	{
		VehicleCoreHoveringCar->HandleControl(EHoveringCarControl_TurnRight, Value);
		return true;
	}
	return false;
}

bool UDrivingComponent::InteractWithPitchUp(float Value)
{
	if (CurrentOverlappingVehicle == nullptr)
	{
		return false;
	}
	return false;
}

bool UDrivingComponent::ToggleVehicle()
{
	if (CurrentOverlappingVehicle == nullptr)
	{
		return false;
	}

	if (CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return CurrentOverlappingVehicle->ShutdownVehicle();
	}
	else
	{
		return CurrentOverlappingVehicle->StartupVehicle();
	}
}
