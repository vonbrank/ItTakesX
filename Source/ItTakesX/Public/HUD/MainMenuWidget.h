// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
	UFUNCTION()
	void StartButtonClicked();
	UFUNCTION()
	void QuitButtonClicked();
	void MenuTearDown();
protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

public:
	UFUNCTION(BlueprintCallable)
	void MenuSetup();
};
