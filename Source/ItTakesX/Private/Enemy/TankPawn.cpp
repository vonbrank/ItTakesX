// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TankPawn.h"

#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

ATankPawn::ATankPawn()
{
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(RootComponent);
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ATankPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsTargetInRange())
	{
		if (TimeToNextShoot > 0)
		{
			TimeToNextShoot -= DeltaSeconds;
		}
		else
		{
			Shoot();
			TimeToNextShoot = ShootInterval;
		}
	}
	else
	{
		TimeToNextShoot = 0;
	}
}

void ATankPawn::LookAtTarget(FVector TargetPosition)
{
	Super::LookAtTarget(TargetPosition);

	FVector LookAtDirection = (TargetPosition - TurretMesh->GetComponentLocation() - FVector(0, 0, 100.f));
	FRotator PreviewRotation = TurretMesh->GetComponentRotation();
	FRotator NewRotation = LookAtDirection.ToOrientationRotator();
	// NewRotation.Pitch = NewRotation.Roll = 0;
	TurretMesh->SetWorldRotation(
		FMath::RInterpTo(PreviewRotation, NewRotation,
		                 UGameplayStatics::GetWorldDeltaSeconds(this), 2));

	if (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(TurretMesh->GetUpVector(), BodyMesh->GetUpVector()))) >
		30)
	{
		TurretMesh->SetWorldRotation(PreviewRotation);
	}
}

void ATankPawn::Shoot()
{
	GetWorld()->SpawnActor<AStraightProjectileActor>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(),
	                                                 ProjectileSpawnPoint->GetComponentRotation());
}
