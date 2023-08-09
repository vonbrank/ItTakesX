// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBasePawn.h"
#include "ChariotPawn.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AChariotPawn : public AEnemyBasePawn
{
	GENERATED_BODY()
public:
	AChariotPawn();
	virtual void Tick(float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	float MaxSpeed = 200.f;

	UPROPERTY(EditAnywhere)
	float Accerleration = 1.f;

	UPROPERTY(VisibleAnywhere)
	TArray<class UStaticMeshComponent*> WeaponMeshList;

	void MoveToForward();
protected:
	virtual void LookAtTarget(FVector TargetPosition) override;
};
