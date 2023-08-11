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
public:
	void ToggleOpenFire();

	virtual void SetIsRunning(bool bNewIsRunning) override;
};
