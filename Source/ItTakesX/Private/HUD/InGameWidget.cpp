// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/InGameWidget.h"

#include "Ability/DrivingComponent.h"
#include "Ability/HealthComponent.h"
#include "Character/ItTakesXCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


bool UInGameWidget::Initialize()
{
	Super::Initialize();

	Character = Cast<AItTakesXCharacter>(GetOwningPlayerPawn());

	if (HealthBar)
	{
		HealthBar->PercentDelegate.BindDynamic(this, &ThisClass::GetHealth);
	}

	if (HealthContainText)
	{
		HealthContainText->TextDelegate.BindDynamic(this, &ThisClass::GetHealthContainText);
	}

	if (CarHealthBar)
	{
		CarHealthBar->PercentDelegate.BindDynamic(this, &ThisClass::GetCarHealth);
	}
	if (CarHealthContainText)
	{
		CarHealthContainText->TextDelegate.BindDynamic(this, &ThisClass::GetCarContainText);
	}

	return true;
}


float UInGameWidget::GetHealth()
{
	if (Character)
	{
		return Character->Health->GetHealthPercentage();
	}
	return 0;
}

FText UInGameWidget::GetHealthContainText()
{
	if (Character)
	{
		return FText::FromString(
			FString::Printf(TEXT("%.f/%.f"), Character->Health->GetHealth(), Character->Health->GetMaxHealth()));
	}
	return FText::FromString(FString::Printf(TEXT("N/A")));
}

float UInGameWidget::GetCarHealth()
{
	if (Character)
	{
		return Character->Driving->GetDrivingVehicleHealthPercentage();
	}
	return 0.5;
}

FText UInGameWidget::GetCarContainText()
{
	if (Character)
	{
		return FText::FromString(
			FString::Printf(TEXT("%.f/%.f"), Character->Driving->GetDrivingVehicleHealth(),
			                Character->Driving->GetDrivingVehicleHealth()));
	}
	return FText::FromString(FString::Printf(TEXT("N/A")));
}
