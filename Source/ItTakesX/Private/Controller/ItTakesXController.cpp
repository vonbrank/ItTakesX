// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ItTakesXController.h"

#include "Character/ItTakesXCharacter.h"

void AItTakesXController::BeginPlay()
{
	Super::BeginPlay();

	UInGameWidget* InGameWidget = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
	if (InGameWidget)
	{
		InGameWidget->SetOwningPlayer(this);
		InGameWidget->AddToViewport();
	}

	CharacterRespawnWidget = CreateWidget<UCharacterRespawnWidget>(this, CharacterRespawnWidgetClass);
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
