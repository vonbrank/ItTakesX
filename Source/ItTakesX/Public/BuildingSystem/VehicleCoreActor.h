// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleComponentActor.h"
#include "VehicleCoreActor.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleCoreActor : public AVehicleComponentActor
{
	GENERATED_BODY()


private:
	UPROPERTY()
	class AItTakesXCharacter* CurrentOverlappingCharacter;

protected:
	virtual void OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	virtual bool PropagateCommand(FVehicleCoreCommand Command) override;
};
