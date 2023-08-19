// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HUD/CharacterRespawnWidget.h"
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
protected:
	virtual void BeginPlay() override;
private:
	float GameTimeInSeconds = 0;

	int CurrentCrossedCircleNumber = 0;

	int CurrentKilledEnemyNumber = 0;

	UPROPERTY()
	TArray<TScriptInterface<IVehicleNode>> CurrentUsedVehicleComponents;

	UPROPERTY()
	class ACheckPoint* LastCheckPoint;

	UPROPERTY()
	class AItTakesXCharacter* Character;

	FTransform CharacterBeginTransform;

	// UPROPERTY()
	// class AItTakesXCharacter* CurrentDeadCharacter;

	// void RespawnDeadCharacter();

	UPROPERTY()
	class AItTakesXController* ItTakesXController;

public:
	FORCEINLINE float GetGameTimeInSeconds() { return GameTimeInSeconds; }
	FORCEINLINE int GetCurrentCrossedCircleNumber() { return CurrentCrossedCircleNumber; }
	FORCEINLINE int GetCurrentKilledEnemyNumber() { return CurrentKilledEnemyNumber; }
	FORCEINLINE int GetCurrentUsedVehicleComponentNumber() { return CurrentUsedVehicleComponents.Num(); }

	void EnemyDied(class AEnemyBasePawn* EnemyBasePawn);
	void CircleCrossed(class AFlyingCircle* FlyingCircle);
	void NewUseVehicleComponents(TArray<TScriptInterface<IVehicleNode>> NewVehicleNodes);
	void SetCharacterBeginTransform(class AItTakesXCharacter* BeginCharacter, FTransform Transform);
	void ArriveCheckPoint(class ACheckPoint* NewCheckPoint);
	void CharacterDied(class AItTakesXCharacter* DeadCharacter);
	void OnWinGameCheckPointRise(class ADestinationCheckPoint* DestinationCheckPoint);
};
