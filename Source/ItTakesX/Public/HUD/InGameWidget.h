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
	// UFUNCTION()


	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CarHealthContainText;
	UFUNCTION()
	FText GetCarContainText();

	UPROPERTY()
	class AItTakesXCharacter* Character;


protected:
	virtual bool Initialize() override;
};
