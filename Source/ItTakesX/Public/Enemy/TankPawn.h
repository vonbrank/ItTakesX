// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBasePawn.h"
#include "Projectile/StraightProjectileActor.h"
#include "TankPawn.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API ATankPawn : public AEnemyBasePawn
{
	GENERATED_BODY()
public:
	ATankPawn();
	virtual void Tick(float DeltaSeconds) override;
private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AStraightProjectileActor> ProjectileClass;

	UPROPERTY(EditAnywhere)
	float ShootInterval = 3.f;

	float TimeToNextShoot;

	void Shoot();

protected:
	virtual void LookAtTarget(FVector TargetPosition) override;
};
