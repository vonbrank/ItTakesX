// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ItTakesXController.h"

#include "Character/ItTakesXCharacter.h"
#include "GameModes/ItTakesXGameMode.h"

void AItTakesXController::BeginPlay()
{
	Super::BeginPlay();

	UInGameWidget* InGameWidget = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
	if (InGameWidget)
	{
		InGameWidget->SetOwningPlayer(this);
		if (bShowInGameUI)
		{
			InGameWidget->AddToViewport();
		}
	}

	CharacterRespawnWidget = CreateWidget<UCharacterRespawnWidget>(this, CharacterRespawnWidgetClass);
	WinGameWidget = CreateWidget<UWinGameWidget>(this, WinGameWidgetClass);
}

void AItTakesXController::RespawnCharacterAtTransform()
{
	AItTakesXCharacter* ItTakesXCharacter = Cast<AItTakesXCharacter>(GetPawn());
	if (ItTakesXCharacter)
	{
		ItTakesXCharacter->RespawnAtTransform(CurrentRespawnTransform);
		if (CharacterRespawnWidget)
		{
			CharacterRespawnWidget->RemoveFromParent();
		}
	}
	GetPawn()->EnableInput(this);
}

void AItTakesXController::CharacterDied(FTransform RespawnTransform)
{
	GetPawn()->DisableInput(this);
	CurrentRespawnTransform = RespawnTransform;
	FTimerHandle CharacterRespawnTimerHandle;
	FTimerDelegate CharacterRespawnTimerDelegate = FTimerDelegate::CreateUObject(
		this, &ThisClass::RespawnCharacterAtTransform);
	GetWorldTimerManager().SetTimer(CharacterRespawnTimerHandle, CharacterRespawnTimerDelegate, RespawnDelay,
	                                false);
	if (CharacterRespawnWidget)
	{
		CharacterRespawnWidget->StartCountDown(RespawnDelay);
		CharacterRespawnWidget->AddToViewport();
	}
}

void AItTakesXController::WinGame(AItTakesXGameMode* GameMode)
{
	if (WinGameWidget)
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(WinGameWidget->TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputModeData);
		SetShowMouseCursor(true);
		bShowMouseCursor = true;
		WinGameWidget->SetGameTimeInSeconds(GameMode->GetGameTimeInSeconds());
		WinGameWidget->SetCurrentCrossedCircleNumber(GameMode->GetCurrentCrossedCircleNumber());
		WinGameWidget->SetCurrentKilledEnemyNumber(GameMode->GetCurrentKilledEnemyNumber());
		WinGameWidget->SetDiedTimes(GameMode->GetCurrentDiedTimes());
		WinGameWidget->SetUsedComponentNumber(GameMode->GetCurrentUsedVehicleComponentNumber());
		WinGameWidget->AddToViewport();
		SetPause(true);
	}
}
