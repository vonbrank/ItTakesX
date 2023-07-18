// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/Equippable.h"
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess = "true"))
	float CameraSocketOffsetDefault = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess = "true"))
	float CameraSocketOffsetNiceToAiming = 240.f;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UAimingComponent* Aiming;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UGrabberComponent* Grabber;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UInventoryComponent* Inventory;
	
	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UDrivingComponent* Driving;
	
	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UGlidingComponent* Gliding;

	EItTakesXViewType ItTakesXView = EItTakesXViewType_Normal;

	UFUNCTION()
	void OnCurrentEquippableUpdate(TScriptInterface<IEquippable> NewEquippableInterface);

	UFUNCTION()
	void SwitchGliding();

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	bool bIsSwitching;




public:
	FORCEINLINE class UCameraComponent* GetFollowCamera() { return FollowCamera; }
	FVector GetFollowCameraLocation() const;

	// void EquipMagnet(AMagnet* MagnetToEquip);
	bool HasMagnetEquipped() const;

	void PickUpAndEquip(TScriptInterface<IEquippable> Equippable);

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchToView(EItTakesXViewType NewItTakesXView);

	void SetCurrentOverlappingVehicleCore(class AVehicleCoreActor* VehicleCoreActor);
};
