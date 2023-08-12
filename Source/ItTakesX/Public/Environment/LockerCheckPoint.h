// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/CheckPoint.h"
#include "Interface/VehicleNode.h"
#include "LockerCheckPoint.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API ALockerCheckPoint : public ACheckPoint
{
	GENERATED_BODY()

public:
	ALockerCheckPoint();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* ButtonArea;

	UFUNCTION()
	virtual void OnButtonSphereStartOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	virtual void OnButtonSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);


protected:
	virtual bool CanSpawnTokenCube() override;

	UPROPERTY()
	class AVehicleComponentActor* CurrentOverlappingButtonActor;
	UPROPERTY(EditAnywhere)
	FName NeedCubeTag;
};
