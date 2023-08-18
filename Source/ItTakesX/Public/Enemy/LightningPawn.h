// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBasePawn.h"
#include "Effect/DottedLazer.h"
#include "Environment/BaseExplosion.h"
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

	void Shoot();

	virtual void Destruct(AActor* DestructCauser, AController* DestructInstigator) override;

	UPROPERTY()
	AActor* CurrentTargetActor;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseExplosion> ExplosionClass;

	UPROPERTY(EditAnywhere)
	float LaserDamage = 30;

protected:
	virtual void OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;
public:
};
