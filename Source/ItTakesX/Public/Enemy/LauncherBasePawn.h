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

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* FakeBodyMesh;

	UPROPERTY(EditAnywhere)
	float ShootInterval = 3.f;

	float TimeToNextShoot;

	virtual void Shoot();
	virtual void LookAtTarget(FVector TargetPosition) override;

	virtual void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                         AController* DamageInstigator, AActor* DamageCauser) override;
	virtual void RadialDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
	                               const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser) override;

	bool bHasDestruct = false;

	void Destruct();
};
