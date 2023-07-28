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

public:
	AVehicleCoreActor();

private:
protected:
	UPROPERTY()
	class AItTakesXCharacter* CurrentOverlappingCharacter;

	virtual void OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
	UPROPERTY()
	class APhysicsConstraintActor* CharacterConstraintActor;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* CharacterEnterMark;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* CharacterExistMark;

	UPROPERTY()
	class ACharacter* CurrenAttachCharacter;


public:
	bool IsVehicleStartup() const;
	bool StartupVehicle();
	bool ShutdownVehicle();

	void AttachCharacter(ACharacter* Character);
	void DetachCurrentCharacter();
};
