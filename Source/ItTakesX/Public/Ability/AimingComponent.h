// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AimingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTAKESX_API UAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAimingComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// UPROPERTY()
	// class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MinGrabbingDistance = 0.f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float MaxGrabbingDistance = 10000.f;

	class IAimable* CurrentAimingAimable;

	TArray<FHitResult> CurrentHitResults;

	// FVector GetFollowCameraLocation() const;

	void AimScene();

	FHitResult* GetNearestHitResult() const;

	class IAimable* GetNearestAimingAimable() const;


public:
	IAimable* GetCurrentAimingAimable() const;
};
