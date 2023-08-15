// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LauncherBasePawn.h"
#include "Enemy/EnemyBasePawn.h"
#include "Projectile/StraightProjectileActor.h"
#include "TankPawn.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API ATankPawn : public ALauncherBasePawn
{
	GENERATED_BODY()
public:
	ATankPawn();
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void LookAtTarget(FVector TargetPosition) override;
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* FakeBodyMesh;

	virtual void Destruct() override;
};
