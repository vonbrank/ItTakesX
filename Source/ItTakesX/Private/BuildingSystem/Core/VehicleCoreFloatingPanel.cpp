// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Core/VehicleCoreFloatingPanel.h"

#include "BuildingSystem/Component/VehicleComponentThruster.h"

void AVehicleCoreFloatingPanel::Turn(float Value)
{
	FVector ActorUpVector = FVector::CrossProduct(GetActorRightVector(), GetActorForwardVector());
	Mesh->AddTorqueInDegrees(Value * ActorUpVector * 250, NAME_None, true);
}

void AVehicleCoreFloatingPanel::Throttle(float Value)
{
	for (auto Thruster : Thrusters)
	{
		Thruster->Throttle(Value * 10000);
	}
}

bool AVehicleCoreFloatingPanel::StartupVehicle()
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
	return true;
}
