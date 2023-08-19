// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleComponentActor.h"
#include "VehicleComponentBlade.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleComponentBlade : public AVehicleComponentActor
{
	GENERATED_BODY()

public:
	AVehicleComponentBlade();
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnWeaponHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                 FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY()
	float Damage = 10.f;

	UPROPERTY()
	TSubclassOf<AActor> ExplosionClass;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* RotatingMesh;

	UPROPERTY(EditAnywhere)
	float RotatingSpeed = 10;
};
