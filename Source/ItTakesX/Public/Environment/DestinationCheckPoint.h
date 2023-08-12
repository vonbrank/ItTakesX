// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/LockerCheckPoint.h"
#include "DestinationCheckPoint.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API ADestinationCheckPoint : public ALockerCheckPoint
{
	GENERATED_BODY()

protected:
	virtual void RiseFlag() override;
};
