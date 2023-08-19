// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Core/VehicleCoreWing.h"

void AVehicleCoreWing::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsRunning)
	{
		auto CurrentVelocity = Mesh->GetPhysicsLinearVelocity();
		Mesh->AddForce(-CurrentVelocity * AirDrag, NAME_None, true);

		FVector CurrentForwardSpeed = FVector::DotProduct(Mesh->GetForwardVector(), Mesh->GetPhysicsLinearVelocity()) *
			Mesh->GetForwardVector();

		// FVector OtherSideSpeed = Mesh->GetPhysicsLinearVelocity() - CurrentForwardSpeed;
		// FVector NewSpeed = CurrentForwardSpeed + OtherSideSpeed * 0.9;
		// Mesh->SetPhysicsLinearVelocity(NewSpeed);

		float CurrentForwardSpeedLength = FVector::DotProduct(Mesh->GetForwardVector(),
		                                                      Mesh->GetPhysicsLinearVelocity());

		if (CurrentForwardSpeedLength > MaxForwardSpeed)
		{
			Mesh->AddForce(Mesh->GetUpVector() * MaxLiftingForce, NAME_None, true);
			Mesh->SetPhysicsLinearVelocity(
				Mesh->GetPhysicsLinearVelocity() / CurrentForwardSpeedLength * MaxForwardSpeed);
		}
		else
		{
			Mesh->AddForce(Mesh->GetUpVector() * CurrentForwardSpeedLength / MaxForwardSpeed * MaxLiftingForce,
			               NAME_None, true);
		}


	}
}
