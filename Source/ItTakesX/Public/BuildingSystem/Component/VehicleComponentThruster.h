// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleComponentActor.h"
#include "VehicleComponentThruster.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleComponentThruster : public AVehicleComponentActor
{
	GENERATED_BODY()

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UArrowComponent* BP_ForwardArrowRef;

	UPROPERTY(EditAnywhere)
	float CurrenForceLength;

public:
	void Throttle(float Value);
};
