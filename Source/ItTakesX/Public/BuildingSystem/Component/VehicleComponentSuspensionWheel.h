// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleComponentActor.h"
#include "VehicleComponentSuspensionWheel.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleComponentSuspensionWheel : public AVehicleComponentActor
{
	GENERATED_BODY()

public:
	AVehicleComponentSuspensionWheel();
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(VisibleAnywhere)
	class UPhysicsConstraintComponent* SuspensionConstraint;

	UPROPERTY(VisibleAnywhere)
	class UPhysicsConstraintComponent* AxisConstraint;

	UPROPERTY(VisibleAnywhere)
	class UPhysicsConstraintComponent* TurnConstraint;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* TurnConnection;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* WheelBody;

	void ResetSuspensionDirection();

protected:
public:
	void Throttle(float Value, FVector VehicleForwardVector = FVector::ForwardVector,
	              FVector VehicleRightVector = FVector::RightVector);
	void Turn(float Value, FTransform VehicleCenterTransform = FTransform::Identity);
	virtual void SetIsRunning(bool bNewIsRunning) override;
};
