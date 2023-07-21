// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Core/VehicleCoreHoveringCar.h"

// bool AVehicleCoreHoveringCar::PropagateCommand(FVehicleCoreCommand Command)
// {
// 	for (auto ChildNodeInterface : ChildNodes)
// 	{
// 		auto ChildNode = ChildNodeInterface.GetInterface();
// 		if (ChildNode)
// 		{
// 			ChildNode->PropagateCommand(Command);
// 		}
// 	}
// 	return true;
// }

void AVehicleCoreHoveringCar::HandleControl(EHoveringCarControl HoveringCarControl, float Value)
{
	if (HoveringCarControl == EHoveringCarControl_MoveForward)
	{
		FVector MovingForce = GetActorForwardVector() * (MaxMovingForce * Value);
		Mesh->AddForce(MovingForce);
	}
	else if (HoveringCarControl == EHoveringCarControl_TurnRight)
	{
		bool bMovingForward = FVector::DotProduct(GetActorForwardVector(), Mesh->GetPhysicsLinearVelocity()) >= 0;
		if (bMovingForward)
		{
			FVector TurningTorque = FVector::DownVector * MaxTurnTorque * Value;
			if (bMovingForward)
			{
				Mesh->AddTorqueInRadians(TurningTorque);
			}
			else
			{
				Mesh->AddTorqueInRadians(-TurningTorque);
			}
		}
	}
}
