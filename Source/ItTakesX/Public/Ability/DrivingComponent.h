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
	class AVehicleControllerActor* CurrentOverlappingVehicle;

	UPROPERTY()
	class AItTakesXCharacter* Character;

protected:
public:
	void SetCurrenOverlappingVehicle(class AVehicleControllerActor* NewOverlappingVehicle);

	bool ToggleVehicle(bool& bOutShouldActiveMovement);
	// bool ExecuteVehicleCommand(FVehicleCoreCommand Command);
	bool InteractWithMoveForward(float Value);
	bool InteractWithTurnRight(float Value);
	bool InteractWithPitchUp(float Value);
	bool Throttle(float Value);
	// bool Pitch(float Value);
	// bool Roll(float Value);
	bool Turn(float Value);

	bool AircraftThrottle(float Value);
	bool AircraftTurn(float Value);
	bool AircraftPitch(float Value);
};
