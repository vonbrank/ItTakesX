// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HUD/CharacterRespawnWidget.h"
#include "HUD/InGameWidget.h"
#include "HUD/WinGameWidget.h"
#include "ItTakesXController.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AItTakesXController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UInGameWidget> InGameWidgetClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UCharacterRespawnWidget> CharacterRespawnWidgetClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UWinGameWidget> WinGameWidgetClass;

	UPROPERTY()
	class UCharacterRespawnWidget* CharacterRespawnWidget;

	UPROPERTY()
	class UWinGameWidget* WinGameWidget;

	FTransform CurrentRespawnTransform;

	UPROPERTY()
	float RespawnDelay = 3;


	UPROPERTY(EditDefaultsOnly)
	bool bShowInGameUI = true;

protected:
	void RespawnCharacterAtTransform();

public:
	void CharacterDied(FTransform RespawnTransform);
	UFUNCTION(BlueprintCallable)
	void WinGame(class AItTakesXGameMode* GameMode);
};
