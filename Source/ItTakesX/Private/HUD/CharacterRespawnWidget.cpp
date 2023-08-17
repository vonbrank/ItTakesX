// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterRespawnWidget.h"

#include "Components/TextBlock.h"

void UCharacterRespawnWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CurrentCountDownTime >= 0)
	{
		CurrentCountDownTime -= InDeltaTime;
	}
}

bool UCharacterRespawnWidget::Initialize()
{
	Super::Initialize();

	if (RespawnTextBlock)
	{
		RespawnTextBlock->TextDelegate.BindDynamic(this, &ThisClass::GetRespawnText);
	}

	return true;
}

FText UCharacterRespawnWidget::GetRespawnText()
{
	if (CurrentCountDownTime < 1)
	{
		return FText::FromString(FString::Printf(TEXT("开始!")));
	}
	return FText::FromString(FString::Printf(TEXT("您将在 %.f 秒内重生!"), CurrentCountDownTime));
}

void UCharacterRespawnWidget::StartCountDown(float CountDownTime)
{
	CurrentCountDownTime = CountDownTime;
}
