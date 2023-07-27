// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Core/VehicleCoreSimpleAirplane.h"

void AVehicleCoreSimpleAirplane::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector CurrenAngularVelocity = Mesh->GetPhysicsAngularVelocityInDegrees();
	Mesh->AddTorqueInDegrees(-CurrenAngularVelocity / 0.75, NAME_None, true);

	SpeedUp();
}

void AVehicleCoreSimpleAirplane::Throttle(float Value)
{
	CurrentThrottle += Value;
	CurrentThrottle = FMath::Clamp(CurrentThrottle, 0, MaxThrottle);
}

void AVehicleCoreSimpleAirplane::Pitch(float Value)
{
	Mesh->AddTorqueInDegrees(Value * GetActorRightVector() * (CurrentThrottle / MaxThrottle) * 1250, NAME_None, true);
}

void AVehicleCoreSimpleAirplane::Roll(float Value)
{
	Mesh->AddTorqueInDegrees(Value * GetActorForwardVector() * (CurrentThrottle / MaxThrottle) * 1250, NAME_None, true);
}

void AVehicleCoreSimpleAirplane::Yaw(float Value)
{
	FVector ActorUpVector = FVector::CrossProduct(GetActorRightVector(), GetActorForwardVector());
	Mesh->AddTorqueInDegrees(Value * ActorUpVector * (CurrentThrottle / MaxThrottle) * 1250, NAME_None, true);
}

void AVehicleCoreSimpleAirplane::SpeedUp()
{
	FVector NewSpeed = FMath::Lerp(Mesh->GetPhysicsLinearVelocity(), GetActorForwardVector() * CurrentThrottle * 50,
	                               0.1);
	Mesh->SetPhysicsLinearVelocity(NewSpeed);
}
