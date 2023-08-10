// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TurretPawn.h"

#include "Kismet/GameplayStatics.h"

void ATurretPawn::LookAtTarget(FVector TargetPosition)
{
	Super::LookAtTarget(TargetPosition);

	FVector LookAtDirection = (TargetPosition - TurretMesh->GetComponentLocation());
	FRotator NewRotation = LookAtDirection.ToOrientationRotator();
	NewRotation.Pitch = TurretMesh->GetComponentRotation().Pitch;
	NewRotation.Roll = TurretMesh->GetComponentRotation().Roll;
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(TurretMesh->GetComponentRotation(), NewRotation,
		                 UGameplayStatics::GetWorldDeltaSeconds(this), 5));
}
