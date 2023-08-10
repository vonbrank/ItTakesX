// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleComponentActor.h"
#include "VehicleControllerActor.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleControllerActor : public AVehicleComponentActor
{
	GENERATED_BODY()

public:
	AVehicleControllerActor();

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

	UPROPERTY()
	TArray<TScriptInterface<IVehicleNode>> CurrentVehicleNodes;

public:
	bool IsVehicleStartup() const;
	virtual bool StartupVehicle();
	virtual bool ShutdownVehicle();

	void AttachCharacter(ACharacter* Character);
	void DetachCurrentCharacter();

	void Throttle(float Value);
	void Turn(float Value);

	void AircraftThrottle(float Value);
	void AircraftTurn(float Value);
};
