// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ChariotPawn.h"

#include "Kismet/GameplayStatics.h"

AChariotPawn::AChariotPawn()
{
	for (int i = 1; i <= 6; i++)
	{
		UStaticMeshComponent* WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(
			*FString::Printf(TEXT("WeaponMesh%d"), i));
		WeaponMesh->SetupAttachment(RootComponent);
		WeaponMeshList.Add(WeaponMesh);
	}
}

void AChariotPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AChariotPawn::LookAtTarget(FVector TargetPosition)
{
	Super::LookAtTarget(TargetPosition);


	FVector VectorToTarget = TargetPosition - BodyMesh->GetComponentLocation();

	FVector NormalVector = BodyMesh->GetUpVector();

	FVector TargetForwardVector = VectorToTarget +
		NormalVector * (FVector::DotProduct(VectorToTarget, NormalVector) * -1);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("ForwardVector = %s"), *BodyMesh->GetForwardVector().ToString()));
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("TargetForwardVector = %s"), *TargetForwardVector.ToString()));
	TargetForwardVector.Normalize();
	float DiffAngle = FMath::RadiansToDegrees(
		FMath::Acos(FVector::DotProduct(TargetForwardVector, BodyMesh->GetForwardVector())));

	if (FVector::DotProduct(FVector::UpVector,
	                        FVector::CrossProduct(TargetForwardVector, BodyMesh->GetForwardVector())) > 0)
	{
		BodyMesh->AddTorqueInRadians(FVector::UpVector * -50, NAME_None, true);
	}
	else
	{
		BodyMesh->AddTorqueInRadians(FVector::UpVector * 50, NAME_None, true);
	}
}
