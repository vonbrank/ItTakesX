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

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float MaxSpeed = 200.f;

	UPROPERTY(EditAnywhere)
	float Accerleration = 1.f;

	UPROPERTY(VisibleAnywhere)
	TArray<class UStaticMeshComponent*> WeaponMeshList;

	UFUNCTION()
	void OnWeaponHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                 FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	float WeaponDamage = 10;
	//
	// bool bHasDestroy;

protected:
	virtual void LookAtTarget(FVector TargetPosition) override;

	virtual void Destruct(AActor* DestructCauser, AController* DestructInstigator) override;

public:
	UFUNCTION(BlueprintImplementableEvent)
	void BreakPhysicsConstraintEvent();
};
