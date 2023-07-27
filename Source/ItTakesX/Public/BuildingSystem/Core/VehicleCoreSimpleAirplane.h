// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleCoreActor.h"
#include "VehicleCoreSimpleAirplane.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleCoreSimpleAirplane : public AVehicleCoreActor
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	float MaxThrottle = 750.f;
	float CurrentThrottle;

	void SpeedUp();
public:
	void Throttle(float Value);
	void Pitch(float Value);
	void Roll(float Value);
	void Yaw(float Value);
};
