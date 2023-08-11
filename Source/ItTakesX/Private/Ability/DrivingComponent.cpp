// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/DrivingComponent.h"

#include "BuildingSystem/VehicleControllerActor.h"
#include "BuildingSystem/Component/VehicleComponentThruster.h"
#include "BuildingSystem/Core/VehicleCoreFloatingPanel.h"
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

void UDrivingComponent::SetCurrenOverlappingVehicle(AVehicleControllerActor* NewOverlappingVehicle)
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

bool UDrivingComponent::ToggleVehicle(bool& bOutShouldActiveMovement)
{
	bOutShouldActiveMovement = false;

	if (CurrentOverlappingVehicle == nullptr)
	{
		return false;
	}

	if (CurrentOverlappingVehicle->IsVehicleStartup())
	{
		bool bShutdownResult = CurrentOverlappingVehicle->ShutdownVehicle();
		if (Character)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			                                 FString::Printf(
				                                 TEXT("location of character %s"),
				                                 *Character->GetActorLocation().ToString()));

			CurrentOverlappingVehicle->DetachCurrentCharacter();
			bOutShouldActiveMovement = true;

			// Character->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			// Character->SetActorLocation(CurrentOverlappingVehicle->GetActorLocation() + FVector::UpVector * 100);
			// Character->GetMovementComponent()->Velocity = FVector::Zero();
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			                                 FString::Printf(
				                                 TEXT("location of character %s"),
				                                 *Character->GetActorLocation().ToString()));
			// Character->();
		}
		return bShutdownResult;
	}
	else
	{
		CurrentOverlappingVehicle->AttachCharacter(Character);

		return CurrentOverlappingVehicle->StartupVehicle();
	}
}

bool UDrivingComponent::Throttle(float Value)
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}
	//
	// AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
	// if (VehicleCoreQuadcopter)
	// {
	// 	VehicleCoreQuadcopter->Throttle(Value);
	// 	return true;
	// }
	//
	// AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
	// if (VehicleCoreSimpleAirplane)
	// {
	// 	VehicleCoreSimpleAirplane->Throttle(Value);
	// 	return true;
	// }
	//
	// AVehicleCoreFloatingPanel* VehicleCoreFloatingPanel = Cast<AVehicleCoreFloatingPanel>(CurrentOverlappingVehicle);
	// if (VehicleCoreFloatingPanel)
	// {
	// 	VehicleCoreFloatingPanel->Throttle(Value);
	// 	return true;
	// }

	if (CurrentOverlappingVehicle)
	{
		CurrentOverlappingVehicle->Throttle(Value);
		return true;
	}

	return false;
}

// bool UDrivingComponent::Pitch(float Value)
// {
// 	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
// 	{
// 		return false;
// 	}
//
// 	AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
// 	if (VehicleCoreQuadcopter)
// 	{
// 		VehicleCoreQuadcopter->Pitch(Value);
// 		return true;
// 	}
//
// 	AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
// 	if (VehicleCoreSimpleAirplane)
// 	{
// 		VehicleCoreSimpleAirplane->Pitch(Value);
// 		return true;
// 	}
//
// 	return false;
// }
//
// bool UDrivingComponent::Roll(float Value)
// {
// 	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
// 	{
// 		return false;
// 	}
//
// 	AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
// 	if (VehicleCoreQuadcopter)
// 	{
// 		VehicleCoreQuadcopter->Roll(Value);
// 		return true;
// 	}
//
// 	AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
// 	if (VehicleCoreSimpleAirplane)
// 	{
// 		VehicleCoreSimpleAirplane->Roll(Value);
// 		return true;
// 	}
//
// 	return false;
// }

bool UDrivingComponent::Turn(float Value)
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}
	//
	// AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
	// if (VehicleCoreQuadcopter)
	// {
	// 	VehicleCoreQuadcopter->Yaw(Value);
	// 	return true;
	// }
	//
	// AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
	// if (VehicleCoreSimpleAirplane)
	// {
	// 	VehicleCoreSimpleAirplane->Yaw(Value);
	// 	return true;
	// }
	//
	// AVehicleCoreFloatingPanel* VehicleCoreFloatingPanel = Cast<AVehicleCoreFloatingPanel>(CurrentOverlappingVehicle);
	// if (VehicleCoreFloatingPanel)
	// {
	// 	VehicleCoreFloatingPanel->Turn(Value);
	// 	return true;
	// }

	if (CurrentOverlappingVehicle)
	{
		CurrentOverlappingVehicle->Turn(Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::AircraftThrottle(float Value)
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentOverlappingVehicle)
	{
		CurrentOverlappingVehicle->AircraftThrottle(Value * 10);
		return true;
	}

	return false;
}

bool UDrivingComponent::AircraftTurn(float Value)
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentOverlappingVehicle)
	{
		CurrentOverlappingVehicle->AircraftTurn(Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::AircraftPitch(float Value)
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentOverlappingVehicle)
	{
		CurrentOverlappingVehicle->AircraftPitch(Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::ToggleOpenFire()
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentOverlappingVehicle)
	{
		CurrentOverlappingVehicle->ToggleOpenFire();
		return true;
	}

	return false;
}

bool UDrivingComponent::LaunchProjectile()
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentOverlappingVehicle)
	{
		CurrentOverlappingVehicle->LaunchProjectile();
		return true;
	}

	return false;
}

bool UDrivingComponent::VerticalRotateTurret(float Value)
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentOverlappingVehicle)
	{
		CurrentOverlappingVehicle->VerticalRotateTurret(Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::HorizontalRotateTurret(float Value)
{
	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentOverlappingVehicle)
	{
		CurrentOverlappingVehicle->HorizontalRotateTurret(Value);
		return true;
	}

	return false;
}
