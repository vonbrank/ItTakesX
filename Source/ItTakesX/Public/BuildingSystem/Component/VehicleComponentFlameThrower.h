// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleComponentActor.h"
#include "Particles/Emitter.h"
#include "VehicleComponentFlameThrower.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleComponentFlameThrower : public AVehicleComponentActor
{
	GENERATED_BODY()
public:
	AVehicleComponentFlameThrower();
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* FireParticleSpawnPoint;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* FireArea;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEmitter> FireParticleClass;

	UPROPERTY()
	AEmitter* CurrentFireParticle;

	UPROPERTY()
	float DamagePerSecond = 30.f;

	UFUNCTION()
	virtual void OnFireAreaStartOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	virtual void OnFireAreaEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UPROPERTY()
	TArray<class AEnemyBasePawn*> CurrentOverlappingEnemy;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEmitter> FireClass;

public:
	void ToggleOpenFire();

	virtual void SetIsRunning(bool bNewIsRunning) override;

	FORCEINLINE TSubclassOf<AEmitter> GetFireClass() { return FireClass; }
};
