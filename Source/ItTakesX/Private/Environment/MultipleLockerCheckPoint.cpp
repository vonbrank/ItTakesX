// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/MultipleLockerCheckPoint.h"

#include "BuildingSystem/VehicleComponentActor.h"

bool AMultipleLockerCheckPoint::CanSpawnTokenCube()
{
	if (CurrentOverlappingButtonActor == nullptr)
	{
		return false;
	}

	auto AllVehicleComponents = CurrentOverlappingButtonActor->GetVehicleRoot()->GetAllChildNodes();

	bool bHasTag1 = false;
	bool bHasTag2 = false;

	for (auto VehicleNode : AllVehicleComponents)
	{
		auto VehicleNodeActor = Cast<AActor>(VehicleNode.GetInterface());
		if (VehicleNodeActor)
		{
			if (VehicleNodeActor->Tags.Contains(NeedCubeTag))
			{
				bHasTag1 = true;
			}
			else if (VehicleNodeActor->Tags.Contains(NeedCubeTag2))
			{
				bHasTag2 = true;
			}
		}
	}

	return bHasTag1 && bHasTag2 && ACheckPoint::CanSpawnTokenCube();
}
