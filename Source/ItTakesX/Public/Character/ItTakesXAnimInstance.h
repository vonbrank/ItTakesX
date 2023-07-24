// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ItTakesXAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API UItTakesXAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	UPROPERTY(BlueprintReadOnly, Category = INVALID_NAME_CHARACTERS, meta = (AllowPrivateAccess = "true"))
	class AItTakesXCharacter* ItTakesXCharacter;

	UPROPERTY(BlueprintReadOnly, Category = INVALID_NAME_CHARACTERS, meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = INVALID_NAME_CHARACTERS, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = INVALID_NAME_CHARACTERS, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = INVALID_NAME_CHARACTERS, meta = (AllowPrivateAccess = "true"))
	bool bEquippingMagnet;
	UPROPERTY(BlueprintReadOnly, Category = INVALID_NAME_CHARACTERS, meta = (AllowPrivateAccess = "true"))
	bool bEquippingWeapon;

	UPROPERTY(BlueprintReadOnly, Category = INVALID_NAME_CHARACTERS, meta = (AllowPrivateAccess = "true"))
	bool bIsGliding;

	UPROPERTY(BlueprintReadOnly, Category = INVALID_NAME_CHARACTERS, meta = (AllowPrivateAccess = "true"))
	float YawOffset;
};
