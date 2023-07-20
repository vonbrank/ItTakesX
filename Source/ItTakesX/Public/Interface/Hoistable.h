// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Hoistable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHoistable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ITTAKESX_API IHoistable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnBeginHoisting(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnEndHoisting(AActor* OtherActor);

	virtual bool IsHoisting() const = 0;
};
