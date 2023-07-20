// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Core/VehicleCoreQuadcopter.h"

bool AVehicleCoreQuadcopter::PropagateCommand(FVehicleCoreCommand Command)
{
	for (auto ChildNodeInterface : ChildNodes)
	{
		auto ChildNode = ChildNodeInterface.GetInterface();
		if (ChildNode)
		{
			ChildNode->PropagateCommand(Command);
		}
	}
	return true;
}
