// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBasePawn.h"
#include "Field/FieldSystemActor.h"
#include "Projectile/StraightProjectileActor.h"
#include "LauncherBasePawn.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API ALauncherBasePawn : public AEnemyBasePawn
{
	GENERATED_BODY()

public:
	ALauncherBasePawn();
	virtual void Tick(float DeltaSeconds) override;
private:
	UPROPERTY(EditAnywhere)
	float ProjectileLifeSpan = 8.f;

	UPROPERTY(EditAnywhere)
	float HomingAccelerationMagnitude = 0;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseProjectile> ProjectileClass;

	UPROPERTY(VisibleAnywhere)
	class UGeometryCollectionComponent* TurretGeometryCollection;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFieldSystemActor> FieldSystemClass;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* FieldSystemSpawnPoint;

	UPROPERTY(EditAnywhere)
	float ShootInterval = 3.f;

	float TimeToNextShoot;

	UPROPERTY(EditAnywhere)
	float ProjectileSpeed = 1500;

	virtual void Shoot();
	virtual void LookAtTarget(FVector TargetPosition) override;

	virtual void Destruct(AActor* DestructCauser, AController* DestructInstigator) override;
};
