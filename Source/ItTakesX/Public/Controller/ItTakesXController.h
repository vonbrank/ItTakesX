// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
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
};
