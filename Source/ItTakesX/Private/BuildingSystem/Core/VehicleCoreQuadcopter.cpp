// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Core/VehicleCoreQuadcopter.h"

#include "BuildingSystem/Component/VehicleComponentThruster.h"
#include "PhysicsEngine/PhysicsSettings.h"

void AVehicleCoreQuadcopter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsRunning)
	{
		FVector ActorUpVector = FVector::CrossProduct(GetActorForwardVector(), GetActorRightVector());
		// Mesh->AddForce(ActorUpVector * 980.f, NAME_None, true);

		float AngleRadians = FMath::Acos(FVector::DotProduct(ActorUpVector, FVector::UpVector));
		float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);

		float alpha = AngleDegrees / RollBackMaxDegrees;
		FVector RollBackVector = FVector::CrossProduct(ActorUpVector, FVector::UpVector);
		//
		Mesh->AddTorqueInRadians(RollBackVector * RollBackTorqueLength * alpha, NAME_None, true);
		Mesh->SetAngularDamping(10);

		// if (AngleDegrees > 5)
		// {
		// 	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		// 	//                                  FString::Printf(TEXT("ActorUpVector = %s"), *ActorUpVector.ToString()));
		//
		// }
	}


	// Mesh->Add
}

void AVehicleCoreQuadcopter::Throttle(float Value)
{
	// FVector ActorUpVector = FVector::CrossProduct(GetActorForwardVector(), GetActorRightVector());
	// Mesh->AddForce(ActorUpVector * ThrottleForceLength * Value, NAME_None, true);

	for (auto Thruster : Thrusters)
	{
		Thruster->Throttle(Value * ThrottleForceLength * 10);
	}
}

void AVehicleCoreQuadcopter::Pitch(float Value)
{
	Mesh->AddTorqueInRadians(GetActorRightVector() * PitchTorqueLength * Value, NAME_None, true);
}

void AVehicleCoreQuadcopter::Roll(float Value)
{
	Mesh->AddTorqueInRadians(GetActorForwardVector() * PitchTorqueLength * Value, NAME_None, true);
}

void AVehicleCoreQuadcopter::Yaw(float Value)
{
	Mesh->AddTorqueInRadians(-FVector::UpVector * PitchTorqueLength * 2 * Value, NAME_None, true);
}

bool AVehicleCoreQuadcopter::StartupVehicle()
{
	Super::StartupVehicle();

	Thrusters.Reset();

	for (auto VehicleNodeInterface : ChildNodes)
	{
		auto VehicleNode = VehicleNodeInterface.GetInterface();
		AVehicleComponentThruster* VehicleComponentThruster = Cast<AVehicleComponentThruster>(VehicleNode);
		if (VehicleComponentThruster)
		{
			Thrusters.Add(VehicleComponentThruster);
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(TEXT("Vehicle has %d thrusters"), Thrusters.Num()));

	return true;
}
