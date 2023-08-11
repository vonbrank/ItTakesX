// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/VehicleComponentTurret.h"

#include "Components/ArrowComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AVehicleComponentTurret::AVehicleComponentTurret()
{
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void AVehicleComponentTurret::BeginPlay()
{
	Super::BeginPlay();

	LaunchProjectile();
}

void AVehicleComponentTurret::LaunchProjectile()
{
	auto ProjectileActor = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass,
	                                                               ProjectileSpawnPoint->GetComponentLocation(),
	                                                               ProjectileSpawnPoint->GetComponentRotation());
	if (ProjectileActor)
	{
		FVector CurrentSpeed = Mesh->GetPhysicsLinearVelocity();

		ProjectileActor->SetActorScale3D(FVector(1, 1, 1));
		ProjectileActor->GetProjectileMovement()->InitialSpeed = ProjectileSpeed + FVector::DotProduct(
			CurrentSpeed, ProjectileSpawnPoint->GetForwardVector());
		ProjectileActor->GetProjectileMovement()->MaxSpeed = ProjectileSpeed + FVector::DotProduct(
			CurrentSpeed, ProjectileSpawnPoint->GetForwardVector());
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Ref = %s"), *Ref->GetName()));
}

void AVehicleComponentTurret::VerticalRotateTurret(float Value)
{
	auto NewRotation = TurretMesh->GetRelativeRotation();
	NewRotation.Add(GetWorld()->DeltaTimeSeconds * TurretRotatingSpeedDegree * Value, 0, 0);


	if (NewRotation.Pitch < 0)
	{
		NewRotation.Pitch = 0;
	}
	else if (NewRotation.Pitch > MaxVerticalRotatingDegree)
	{
		NewRotation.Pitch = MaxVerticalRotatingDegree;
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("NewPitch = %f"), NewRotation.Pitch));
	TurretMesh->SetRelativeRotation(NewRotation);
}

void AVehicleComponentTurret::HorizontalRotateTurret(float Value)
{
	auto NewRotation = TurretMesh->GetRelativeRotation();
	NewRotation.Add(0, GetWorld()->DeltaTimeSeconds * TurretRotatingSpeedDegree * -Value, 0);
	TurretMesh->SetRelativeRotation(NewRotation);
}
