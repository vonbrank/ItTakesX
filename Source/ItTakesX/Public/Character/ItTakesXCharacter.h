// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItTakesXCharacter.generated.h"

UCLASS()
class ITTAKESX_API AItTakesXCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AItTakesXCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	void Zoom(float Value);

	void HandlePressingE();
	void HandlePressingF();
	void HandleRepeatingLShift();
	void HandleRepeatingLCtrl();
	void HandleRepeatingZ();
	void HandleRepeatingX();


private:
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UAimingComponent* Aiming;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UGrabberComponent* Grabber;

public:
	FORCEINLINE class UCameraComponent* GetFollowCamera() { return FollowCamera; }
	FVector GetFollowCameraLocation() const;
};
