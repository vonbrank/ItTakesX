// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/VehicleComponentTurret.h"

#include "Components/ArrowComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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

	// LaunchProjectile();
}

void AVehicleComponentTurret::LaunchProjectile()
{
	auto ProjectileActor = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass,
	                                                               ProjectileSpawnPoint->GetComponentLocation(),
	                                                               ProjectileSpawnPoint->GetComponentRotation());
	if (ProjectileActor)
	{
		FVector CurrentSpeed = Mesh->GetPhysicsLinearVelocity();

		ProjectileActor->SetActorScale3D(FVector(ProjectileScale, ProjectileScale, ProjectileScale));
		ProjectileActor->GetProjectileMovement()->InitialSpeed = ProjectileSpeed + FVector::DotProduct(
			CurrentSpeed, ProjectileSpawnPoint->GetForwardVector());
		ProjectileActor->GetProjectileMovement()->MaxSpeed = ProjectileSpeed + FVector::DotProduct(
			CurrentSpeed, ProjectileSpawnPoint->GetForwardVector());
		ProjectileActor->SetOwner(UGameplayStatics::GetPlayerCharacter(this, 0));
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Ref = %s"), *Ref->GetName()));
}

void AVehicleComponentTurret::VerticalRotateTurret(float Value)
{
	auto NewRotation = TurretMesh->GetRelativeRotation();
	NewRotation.Add(GetWorld()->DeltaTimeSeconds * TurretRotatingSpeedDegree * Value, 0, 0);


	if (NewRotation.Pitch < -MaxVerticalRotatingDegree)
	{
		NewRotation.Pitch = -MaxVerticalRotatingDegree;
	}
	else if (NewRotation.Pitch > MaxVerticalRotatingDegree)
	{
		NewRotation.Pitch = MaxVerticalRotatingDegree;
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("NewPitch = %f"), NewRotation.Pitch));
	TurretMesh->SetRelativeRotation(NewRotation);
}

void AVehicleComponentTurret::HorizontalRotateTurret(float Value)
{
	auto NewRotation = TurretMesh->GetRelativeRotation();
	NewRotation.Add(0, GetWorld()->DeltaTimeSeconds * TurretRotatingSpeedDegree * -Value, 0);
	TurretMesh->SetRelativeRotation(NewRotation);
}

void AVehicleComponentTurret::LookAtTarget(FVector TargetLocation)
{
	auto NewRotation = GetTransform().InverseTransformPosition(TargetLocation).
	                                  ToOrientationRotator();

	if (NewRotation.Pitch < -MaxVerticalRotatingDegree)
	{
		NewRotation.Pitch = -MaxVerticalRotatingDegree;
	}
	else if (NewRotation.Pitch > MaxVerticalRotatingDegree)
	{
		NewRotation.Pitch = MaxVerticalRotatingDegree;
	}
	TurretMesh->SetRelativeRotation(FMath::RInterpTo(TurretMesh->GetRelativeRotation(), NewRotation,
	                                                 GetWorld()->DeltaTimeSeconds, 5.f));
}
