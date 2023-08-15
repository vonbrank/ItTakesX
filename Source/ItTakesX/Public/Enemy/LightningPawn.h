// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBasePawn.h"
#include "Effect/DottedLazer.h"
#include "Field/FieldSystemActor.h"
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

	UPROPERTY(VisibleAnywhere)
	class UGeometryCollectionComponent* BodyGeometryCollection;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AFieldSystemActor> FieldSystemClass;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* FieldSystemSpawnPoint;

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

	bool bHasDestruct = false;

	void Shoot();

	void Destruct();

protected:
	virtual void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                         AController* DamageInstigator, AActor* DamageCauser) override;
	virtual void RadialDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, FVector Origin,
	                               const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser) override;

public:
};
