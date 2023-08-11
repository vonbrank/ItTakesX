// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleCoreActor.h"
#include "VehicleCoreWing.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleCoreWing : public AVehicleCoreActor
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere)
	float MaxLiftingForce = 2000;

	UPROPERTY(EditAnywhere)
	float MaxForwardSpeed = 1000;

	UPROPERTY(EditAnywhere)
	float AirDrag = 5;
};
