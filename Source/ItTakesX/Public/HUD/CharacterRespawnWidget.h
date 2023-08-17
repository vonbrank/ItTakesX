// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterRespawnWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API UCharacterRespawnWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool Initialize() override;

private:
	float CurrentCountDownTime;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* RespawnTextBlock;
	UFUNCTION()
	FText GetRespawnText();

public:
	void StartCountDown(float CountDownTime);
};
