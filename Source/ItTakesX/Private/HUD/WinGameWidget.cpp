// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WinGameWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

bool UWinGameWidget::Initialize()
{
	Super::Initialize();


	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &ThisClass::QuitButtonClicked);
	}
	if (ReplayButton)
	{
		ReplayButton->OnClicked.AddDynamic(this, &ThisClass::ReplayButtonClicked);
	}

	if (GameTimeInSecondsTextBlock)
	{
		GameTimeInSecondsTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetGameTimeInSecondsText);
	}
	if (CurrentCrossedCircleNumberBlock)
	{
		CurrentCrossedCircleNumberBlock->TextDelegate.BindDynamic(this, &ThisClass::GetCurrentCrossedCircleNumberText);
	}
	if (CurrentKilledEnemyNumberTextBlock)
	{
		CurrentKilledEnemyNumberTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetCurrentKilledEnemyNumberText);
	}
	if (DiedTimesTextBlock)
	{
		DiedTimesTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetDiedTimesText);
	}

	return true;
}

void UWinGameWidget::QuitButtonClicked()
{
	UGameplayStatics::OpenLevel(this, "MainMenu");
}

void UWinGameWidget::ReplayButtonClicked()
{
	UGameplayStatics::OpenLevel(this, "ItTakesXMapOpenWorld");
}

FText UWinGameWidget::GetGameTimeInSecondsText()
{
	return FText::FromString(FString::Printf(TEXT("消耗时间：%.2f s"), GameTimeInSeconds));
}

FText UWinGameWidget::GetCurrentCrossedCircleNumberText()
{
	return FText::FromString(FString::Printf(TEXT("穿越路径点个数：%d"), CurrentCrossedCircleNumber));
}

FText UWinGameWidget::GetCurrentKilledEnemyNumberText()
{
	return FText::FromString(FString::Printf(TEXT("击杀敌人数：%d"), CurrentKilledEnemyNumber));
}

FText UWinGameWidget::GetDiedTimesText()
{
	return FText::FromString(FString::Printf(TEXT("死亡次数：%d"), DiedTimes));
}
