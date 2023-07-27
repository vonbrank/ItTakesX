// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildingSystem/VehicleCoreActor.h"
#include "Components/ActorComponent.h"
#include "DrivingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTAKESX_API UDrivingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDrivingComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class AVehicleCoreActor* CurrentOverlappingVehicle;

	UPROPERTY()
	class AItTakesXCharacter* Character;

protected:
public:
	void SetCurrenOverlappingVehicle(class AVehicleCoreActor* NewOverlappingVehicle);

	bool ToggleVehicle();
	// bool ExecuteVehicleCommand(FVehicleCoreCommand Command);
	bool InteractWithMoveForward(float Value);
	bool InteractWithTurnRight(float Value);
	bool InteractWithPitchUp(float Value);
	bool Throttle(float Value);
	bool Pitch(float Value);
	bool Roll(float Value);
	bool Yaw(float Value);
};
