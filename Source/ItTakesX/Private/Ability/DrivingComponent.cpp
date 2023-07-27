// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/DrivingComponent.h"

#include "BuildingSystem/Core/VehicleCoreHoveringCar.h"
#include "BuildingSystem/Core/VehicleCoreQuadcopter.h"
#include "BuildingSystem/Core/VehicleCoreSimpleAirplane.h"
#include "Character/ItTakesXCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

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

	Character = Cast<AItTakesXCharacter>(GetOwner());
}


// Called every frame
void UDrivingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentOverlappingVehicle && CurrentOverlappingVehicle->IsVehicleStartup())
	{
		FRotator NewRotation = Character->Controller->GetControlRotation();
		NewRotation.Yaw = CurrentOverlappingVehicle->GetActorRotation().Yaw;
		Character->Controller->SetControlRotation(NewRotation);
	}
}

void UDrivingComponent::SetCurrenOverlappingVehicle(AVehicleCoreActor* NewOverlappingVehicle)
{
	CurrentOverlappingVehicle = NewOverlappingVehicle;
}

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
		bool bShutdownResult = CurrentOverlappingVehicle->ShutdownVehicle();
		if (Character)
		{
			Character->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			// Character->SetActorLocation(CurrentOverlappingVehicle->GetActorLocation());
			Character->GetMovementComponent()->Activate();
			// Character->();
		}
		return bShutdownResult;
	}
	else
	{
		// auto VehicleRootComp = Cast<UPrimitiveComponent>(CurrentOverlappingVehicle->GetRootComponent());

		if (Character)
		{
			Character->AttachToActor(CurrentOverlappingVehicle, FAttachmentTransformRules::KeepWorldTransform);
			Character->GetMovementComponent()->Deactivate();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("character is null")));
		}

		return CurrentOverlappingVehicle->StartupVehicle();
	}
}

bool UDrivingComponent::Throttle(float Value)
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
	if (VehicleCoreQuadcopter)
	{
		VehicleCoreQuadcopter->Throttle(Value);
		return true;
	}

	AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
	if (VehicleCoreSimpleAirplane)
	{
		VehicleCoreSimpleAirplane->Throttle(Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::Pitch(float Value)
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
	if (VehicleCoreQuadcopter)
	{
		VehicleCoreQuadcopter->Pitch(Value);
		return true;
	}

	AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
	if (VehicleCoreSimpleAirplane)
	{
		VehicleCoreSimpleAirplane->Pitch(Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::Roll(float Value)
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
	if (VehicleCoreQuadcopter)
	{
		VehicleCoreQuadcopter->Roll(Value);
		return true;
	}

	AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
	if (VehicleCoreSimpleAirplane)
	{
		VehicleCoreSimpleAirplane->Roll(Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::Yaw(float Value)
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
	if (VehicleCoreQuadcopter)
	{
		VehicleCoreQuadcopter->Yaw(Value);
		return true;
	}

	AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
	if (VehicleCoreSimpleAirplane)
	{
		VehicleCoreSimpleAirplane->Yaw(Value);
		return true;
	}

	return false;
}
