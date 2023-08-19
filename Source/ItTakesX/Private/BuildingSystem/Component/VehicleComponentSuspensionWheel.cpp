// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/VehicleComponentSuspensionWheel.h"

#include "PhysicsEngine/PhysicsConstraintComponent.h"

AVehicleComponentSuspensionWheel::AVehicleComponentSuspensionWheel()
{
	SuspensionConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PC_Suspension"));
	SuspensionConstraint->SetupAttachment(RootComponent);

	AxisConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PC_Axis"));
	AxisConstraint->SetupAttachment(RootComponent);

	TurnConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PC_Turn"));
	TurnConstraint->SetupAttachment(RootComponent);

	TurnConnection = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurnConnection"));
	TurnConnection->SetupAttachment(RootComponent);

	WheelBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WheelBody"));
	WheelBody->SetupAttachment(RootComponent);
}

void AVehicleComponentSuspensionWheel::Throttle(float Value, FVector VehicleForwardVector, FVector VehicleRightVector)
{
	if (bIsRunning)
	{
		FVector VehicleUpVector = FVector::CrossProduct(VehicleForwardVector, VehicleRightVector);
		VehicleUpVector.Normalize();

		FVector WheelUpVector = FVector::CrossProduct(GetActorForwardVector(), VehicleForwardVector);
		WheelUpVector.Normalize();

		if (FVector::DotProduct(VehicleUpVector, WheelUpVector) < 0)
		{
			Value = -Value;
		}

		AxisConstraint->SetAngularVelocityTarget(FVector(Value * 5, 0, 0));
	}
}

void AVehicleComponentSuspensionWheel::Turn(float Value, FTransform VehicleCenterTransform)
{
	FVector RelativePosition = VehicleCenterTransform.InverseTransformPosition(GetActorLocation());

	if (RelativePosition.X > 0)
	{
		Value = -Value;
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("turn value %f"), Value));

	TurnConstraint->SetAngularOrientationTarget(FRotator(0, -Value * 20, 0));
}

void AVehicleComponentSuspensionWheel::SetIsRunning(bool bNewIsRunning)
{
	Super::SetIsRunning(bNewIsRunning);

	if (bNewIsRunning)
	{
		// AxisConstraint->SetAngularVelocityDriveTwistAndSwing(true, true);
		// AxisConstraint->SetAngularDriveParams(50, 1000000, 0);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(TEXT("startup vehicle")));
	}
	else
	{
		// AxisConstraint->SetAngularDriveParams(50, 0, 0);
		// AxisConstraint->SetAngularVelocityDriveTwistAndSwing(false, false);
		AxisConstraint->SetAngularVelocityTarget(FVector(0, 0, 0));
		TurnConstraint->SetAngularOrientationTarget(FRotator(0, 0, 0));
	}
}

void AVehicleComponentSuspensionWheel::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// ResetSuspensionDirection();
}

void AVehicleComponentSuspensionWheel::ResetSuspensionDirection()
{
	FVector ActorUpVector = FVector::CrossProduct(GetActorForwardVector(), GetActorRightVector());
	ActorUpVector.Normalize();

	float AngleToUpVector = FMath::Acos(FVector::DotProduct(ActorUpVector, FVector::UpVector));
	if (FMath::Abs(FMath::RadiansToDegrees(AngleToUpVector)) < 45)
	{
		SuspensionConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Limited, 10);
		SuspensionConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Locked, 10);
	}
	else
	{
		SuspensionConstraint->SetLinearZLimit(ELinearConstraintMotion::LCM_Locked, 10);
		SuspensionConstraint->SetLinearYLimit(ELinearConstraintMotion::LCM_Limited, 10);
	}
}
