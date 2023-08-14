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
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthContainText;

	UPROPERTY()
	class AItTakesXCharacter* Character;

	UFUNCTION()
	float GetHealth();

	UFUNCTION()
	FText GetHealthContainText();

protected:
	virtual bool Initialize() override;
};
