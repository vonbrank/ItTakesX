// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleComponentActor.h"
#include "VehicleComponentHoveringWheel.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleComponentHoveringWheel : public AVehicleComponentActor
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float TraceLength;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float HoverForce;
};
