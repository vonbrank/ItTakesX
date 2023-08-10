// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleComponentActor.h"
#include "VehicleComponentThruster.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleComponentThruster : public AVehicleComponentActor
{
	GENERATED_BODY()

public:
	AVehicleComponentThruster();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* ForwardArrow;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BladeMesh;

	UPROPERTY(EditAnywhere)
	float MaxThrottle = 750;

	float CurrentThrottle = 0;

	UPROPERTY(EditAnywhere)
	float MaxBladeRotatingRate = 2.f;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* WindBarParticle;

public:
	void Throttle(float Value);
	virtual void SetIsRunning(bool bNewIsRunning) override;
};
