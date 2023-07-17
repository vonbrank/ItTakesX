// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Hoistable.h"
#include "GrabberComponent.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
// 	FCurrentHoistableUpdateDelegate, TScriptInterface<IHoistable>, NewHoistable);

UENUM()
enum ERotateDirection
{
	Direction_Up,
	Direction_Down,
	Direction_Left,
	Direction_Right,
	Direction_Max,
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTAKESX_API UGrabberComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGrabberComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UAimingComponent* AimingComponent;

	UPROPERTY()
	class UInventoryComponent* InventoryComponent;

	UPROPERTY()
	TScriptInterface<IHoistable> CurrentHoistingHoistableInterface;

	float CurrentSelectDistance;

	bool InteractWithHoisting();

	ERotateDirection CurrentHoistingObjectRotateDirection = Direction_Max;

	UPROPERTY()
	class ADottedLazer* CurrentMagnetEffect;

	FVector GetHoistingActorHorizontalRotatingAxisRight();


public:
	bool ToggleHoistingActor();
	bool InteractWithComposing();
	bool InteractWithHoistingObjectRotation(ERotateDirection Direction, float Value);
	bool InteractWithZoomingHoistable(float Value);

	// FCurrentHoistableUpdateDelegate OnHoistableUpdate;
};
