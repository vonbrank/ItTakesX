// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API UInGameWidget : public UUserWidget
{
	GENERATED_BODY()

public:
private:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;
	UFUNCTION()
	float GetHealth();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthContainText;
	UFUNCTION()
	FText GetHealthContainText();

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CarHealthBar;
	UFUNCTION()
	float GetCarHealth();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CarHealthContainText;
	UFUNCTION()
	FText GetCarContainText();

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* CarArmourBar;
	UFUNCTION()
	float GetCarArmourBar();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CarArmourContainText;
	UFUNCTION()
	FText GetCarArmourContainText();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyOneTextBlock;
	UFUNCTION()
	FText GetKeyOneText();
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyTwoTextBlock;
	UFUNCTION()
	FText GetKeyTwoText();
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyThreeTextBlock;
	UFUNCTION()
	FText GetKeyThreeText();
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyVTextBlock;
	UFUNCTION()
	FText GetKeyVText();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyETextBlock;
	UFUNCTION()
	FText GetKeyEText();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyFTextBlock;
	UFUNCTION()
	FText GetKeyFText();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyXTextBlock;
	UFUNCTION()
	FText GetKeyXText();

	// UPROPERTY(meta = (BindWidget))
	// class UTextBlock* KeyZTextBlock;
	// UFUNCTION()
	// FText GetKeyZText();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyLShiftTextBlock;
	UFUNCTION()
	FText GetKeyLShiftText();

	// UPROPERTY(meta = (BindWidget))
	// class UTextBlock* KeyLCtrlTextBlock;
	// UFUNCTION()
	// FText GetKeyLCtrlText();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyPageUpTextBlock;
	UFUNCTION()
	FText GetKeyPageUpText();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyPageDownTextBlock;
	UFUNCTION()
	FText GetKeyPageDownText();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyLTextBlock;
	UFUNCTION()
	FText GetKeyLText();

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* KeyKTextBlock;
	UFUNCTION()
	FText GetKeyKText();

	UPROPERTY()
	class AItTakesXCharacter* Character;


protected:
	virtual bool Initialize() override;
};
