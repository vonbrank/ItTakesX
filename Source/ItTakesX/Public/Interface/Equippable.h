// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Equippable.generated.h"

UENUM(BlueprintType)
enum EItTakesXViewType
{
	EItTakesXViewType_Normal,
	EItTakesXViewType_NiceToAiming,
	EItTakesXViewType_NiceToGrabbing,

	EItTakesXViewType_Max
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEquippable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ITTAKESX_API IEquippable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual EItTakesXViewType GetViewType() = 0;
	virtual float GetMaxWalkingSpeed() = 0;
};
