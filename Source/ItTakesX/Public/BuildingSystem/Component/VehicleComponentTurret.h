// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleComponentActor.h"
#include "Environment/BaseExplosion.h"
#include "Projectile/BaseProjectile.h"
#include "VehicleComponentTurret.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleComponentTurret : public AVehicleComponentActor
{
	GENERATED_BODY()
public:
	AVehicleComponentTurret();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere)
	float ProjectileSpeed = 1000;

	UPROPERTY(EditAnywhere)
	float TurretRotatingSpeedDegree = 10;

	UPROPERTY(EditAnywhere)
	float MaxVerticalRotatingDegree = 45;

public:
	void LaunchProjectile();
	void VerticalRotateTurret(float Value);
	void HorizontalRotateTurret(float Value);
};
