// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/LockerCheckPoint.h"
#include "MultipleLockerCheckPoint.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AMultipleLockerCheckPoint : public ALockerCheckPoint
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	FName NeedCubeTag2;
protected:
	virtual bool CanSpawnTokenCube() override;
};
