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

public:
	virtual bool PropagateCommand(FVehicleCoreCommand Command) override;
};
