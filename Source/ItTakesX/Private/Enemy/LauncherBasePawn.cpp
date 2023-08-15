// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/LauncherBasePawn.h"

#include "Components/ArrowComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

ALauncherBasePawn::ALauncherBasePawn()
{
	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(RootComponent);
	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(TurretMesh);

	TurretGeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("BodyGeometryCollection"));
	TurretGeometryCollection->SetupAttachment(RootComponent);


	FieldSystemSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("FieldSystemSpawnPoint"));
	FieldSystemSpawnPoint->SetupAttachment(RootComponent);

	TurretGeometryCollection->SetSimulatePhysics(false);
	TurretGeometryCollection->SetHiddenInGame(true);
	TurretGeometryCollection->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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
	auto Projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass,
	                                                          ProjectileSpawnPoint->GetComponentLocation(),
	                                                          ProjectileSpawnPoint->GetComponentRotation());
	if (Projectile)
	{
		Projectile->SetOwner(this);
	}
}

void ALauncherBasePawn::LookAtTarget(FVector TargetPosition)
{
	Super::LookAtTarget(TargetPosition);
}

void ALauncherBasePawn::Destruct(AActor* DestructCauser, AController* DestructInstigator)
{
	Super::Destruct(DestructCauser, DestructInstigator);

	TurretMesh->SetSimulatePhysics(false);
	TurretMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TurretMesh->SetHiddenInGame(true);

	TurretGeometryCollection->SetHiddenInGame(false);
	TurretGeometryCollection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TurretGeometryCollection->SetSimulatePhysics(true);

	GetWorld()->SpawnActor<AFieldSystemActor>(FieldSystemClass, FieldSystemSpawnPoint->GetComponentLocation(),
	                                          FieldSystemSpawnPoint->GetComponentRotation());
}
