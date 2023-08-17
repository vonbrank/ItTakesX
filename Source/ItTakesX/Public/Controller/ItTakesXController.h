// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HUD/CharacterRespawnWidget.h"
#include "HUD/InGameWidget.h"
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

	UPROPERTY()
	class UCharacterRespawnWidget* CharacterRespawnWidget;

	FTransform CurrentRespawnTransform;

	UPROPERTY()
	float RespawnDelay = 3;

protected:
	void RespawnCharacterAtTransform();

public:
	void CharacterDied(FTransform RespawnTransform);
};
