// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API UWinGameWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
	UFUNCTION()
	void QuitButtonClicked();

	UPROPERTY(meta = (BindWidget))
	class UButton* ReplayButton;
	UFUNCTION()
	void ReplayButtonClicked();

	float GameTimeInSeconds = 0;
	int CurrentCrossedCircleNumber = 0;
	int CurrentKilledEnemyNumber = 0;
	int DiedTimes = 0;
	int UsedComponentNumber = 0;


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* GameTimeInSecondsTextBlock;
	UFUNCTION()
	FText GetGameTimeInSecondsText();
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentCrossedCircleNumberBlock;
	UFUNCTION()
	FText GetCurrentCrossedCircleNumberText();
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CurrentKilledEnemyNumberTextBlock;
	UFUNCTION()
	FText GetCurrentKilledEnemyNumberText();
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DiedTimesTextBlock;
	UFUNCTION()
	FText GetDiedTimesText();
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UsedComponentNumberTextBlock;
	UFUNCTION()
	FText GetUsedComponentNumberText();

public:
	FORCEINLINE void SetGameTimeInSeconds(float Value) { GameTimeInSeconds = Value; }
	FORCEINLINE void SetCurrentCrossedCircleNumber(float Value) { CurrentCrossedCircleNumber = Value; }
	FORCEINLINE void SetCurrentKilledEnemyNumber(float Value) { CurrentKilledEnemyNumber = Value; }
	FORCEINLINE void SetDiedTimes(float Value) { DiedTimes = Value; }
	FORCEINLINE void SetUsedComponentNumber(float Value) { UsedComponentNumber = Value; }
};
