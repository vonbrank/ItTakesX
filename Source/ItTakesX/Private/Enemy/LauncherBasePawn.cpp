// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/LauncherBasePawn.h"

#include "Components/ArrowComponent.h"

ALauncherBasePawn::ALauncherBasePawn()
{
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(RootComponent);
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);
}

void ALauncherBasePawn::Tick(float DeltaSeconds)
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

void ALauncherBasePawn::Shoot()
{
	GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(),
	                                        ProjectileSpawnPoint->GetComponentRotation());
}

void ALauncherBasePawn::LookAtTarget(FVector TargetPosition)
{
	Super::LookAtTarget(TargetPosition);
}
