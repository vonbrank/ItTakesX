// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleCoreActor.h"
#include "VehicleCoreHoveringCar.generated.h"

UENUM()
enum EHoveringCarControl
{
	EHoveringCarControl_MoveForward,
	EHoveringCarControl_TurnRight,

	EHoveringCarControl_Max,
};

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleCoreHoveringCar : public AVehicleCoreActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxMovingForce = 500000.f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxTurnTorque = 500000.f;

public:
	// virtual bool PropagateCommand(FVehicleCoreCommand Command) override;
	virtual void HandleControl(EHoveringCarControl HoveringCarControl, float Value);
};
