// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/VehicleNode.h"
#include "ItTakesXGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AItTakesXGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaSeconds) override;
private:
	float GameTimeInSeconds = 0;

	int CurrentCrossedCircleNumber = 0;

	int CurrentKilledEnemyNumber = 0;

	UPROPERTY()
	TArray<TScriptInterface<IVehicleNode>> CurrentUsedVehicleComponents;

public:
	FORCEINLINE float GetGameTimeInSeconds() { return GameTimeInSeconds; }
	FORCEINLINE int GetCurrentCrossedCircleNumber() { return CurrentCrossedCircleNumber; }
	FORCEINLINE int GetCurrentKilledEnemyNumber() { return CurrentKilledEnemyNumber; }
	FORCEINLINE int GetCurrentUsedVehicleComponentNumber() { return CurrentUsedVehicleComponents.Num(); }

	void EnemyDied(class AEnemyBasePawn* EnemyBasePawn);
	void CircleCrossed(class AFlyingCircle* FlyingCircle);
	void NewUseVehicleComponents(TArray<TScriptInterface<IVehicleNode>> NewVehicleNodes);
};
