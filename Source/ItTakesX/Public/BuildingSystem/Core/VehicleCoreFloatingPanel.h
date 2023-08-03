// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleCoreActor.h"
#include "VehicleCoreFloatingPanel.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleCoreFloatingPanel : public AVehicleCoreActor
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TArray<class AVehicleComponentThruster*> Thrusters;
public:
	void Throttle(float Value);
	void Turn(float Value);
	virtual bool StartupVehicle() override;
};
