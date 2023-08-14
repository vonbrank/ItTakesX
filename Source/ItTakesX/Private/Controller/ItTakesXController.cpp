// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/ItTakesXController.h"

void AItTakesXController::BeginPlay()
{
	Super::BeginPlay();

	UInGameWidget* InGameWidget = CreateWidget<UInGameWidget>(this, InGameWidgetClass);
	if (InGameWidget)
	{
		InGameWidget->SetOwningPlayer(this);
		InGameWidget->AddToViewport();
	}
}
