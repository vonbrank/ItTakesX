// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBasePawn.h"
#include "Effect/DottedLazer.h"
#include "GameFramework/Pawn.h"
#include "LightningPawn.generated.h"

UCLASS()
class ITTAKESX_API ALightningPawn : public AEnemyBasePawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALightningPawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* LightningSpawnLocation;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ADottedLazer> LaserClass;

	UPROPERTY(EditAnywhere)
	float ShootInterval = 5.f;

	float TimeToNextShoot;

	UPROPERTY(EditAnywhere)
	float ShootTime = 0.5;
	float TimeToDestroyLaser;

	UPROPERTY()
	ADottedLazer* CurrentLaser;

	void Shoot();
public:
};
