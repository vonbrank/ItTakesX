// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LauncherBasePawn.h"
#include "Enemy/EnemyBasePawn.h"
#include "TurretPawn.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API ATurretPawn : public ALauncherBasePawn
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float LookUpAngle = 0;

protected:
	virtual void LookAtTarget(FVector TargetPosition) override;
};
