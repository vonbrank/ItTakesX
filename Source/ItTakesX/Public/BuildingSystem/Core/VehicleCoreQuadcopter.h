// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleCoreActor.h"
#include "VehicleCoreQuadcopter.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleCoreQuadcopter : public AVehicleCoreActor
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RollBackTorqueLength = 200.f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RollBackMaxDegrees = 180.f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ThrottleForceLength = 1000.f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float PitchTorqueLength = 10.f;

	UPROPERTY()
	TArray<class AVehicleComponentThruster*> Thrusters;

public:
	virtual void Tick(float DeltaSeconds) override;

	void Throttle(float Value);
	void Pitch(float Value);
	void Roll(float Value);
	void Yaw(float Value);

	virtual bool StartupVehicle() override;
	
};
