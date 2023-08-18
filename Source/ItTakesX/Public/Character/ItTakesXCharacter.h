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

	void Throttle(float Value);
	// void Pitch(float Value);
	// void Roll(float Value);
	void VehicleTurn(float Value);

	void AircraftThrottle(float Value);
	void AircraftTurn(float Value);
	void AircraftPitch(float Value);

	void HandlePressingE();
	void HandlePressingF();
	void HandleRepeatingLShift();
	void HandleRepeatingLCtrl();
	void HandleRepeatingZ();
	void HandleRepeatingX();
	void HandlePressingOne();
	void HandlePressingTwo();
	void HandlePressingThree();
	void HandlePressingLMB();
	void HandlePressingK();
	void HandlePressingL();


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess = "true"))
	// float CameraSocketOffsetDefault = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess = "true"))
	FVector CameraSocketOffsetNiceToAiming = FVector(0, 240.f, 0);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera, meta=(AllowPrivateAccess = "true"))
	FVector CameraSocketOffsetNiceToGrab = FVector(0, 0, 80.f);

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UAimingComponent* Aiming;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UGrabberComponent* Grabber;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UDrivingComponent* Driving;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UGlidingComponent* Gliding;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere, Category = "Ability")
	class UHealthComponent* Health;

	EItTakesXViewType ItTakesXView = EItTakesXViewType_Normal;

	UFUNCTION()
	void OnCurrentEquippableUpdate(TScriptInterface<IEquippable> NewEquippableInterface);

	UFUNCTION()
	void SwitchGliding();

	UPROPERTY(BlueprintReadWrite, meta=(AllowPrivateAccess = "true"))
	bool bIsSwitching;

	friend class UInGameWidget;

	UPROPERTY()
	class AItTakesXGameMode* ItTakesXGameMode;

	bool ToggleDriving();

public:
	FORCEINLINE class UCameraComponent* GetFollowCamera() { return FollowCamera; }
	FVector GetFollowCameraLocation() const;

	// void EquipMagnet(AMagnet* MagnetToEquip);
	bool HasMagnetEquipped() const;
	bool IsGliding() const;
	bool HasWeaponEquipped() const;

	void PickUpAndEquip(TScriptInterface<IEquippable> Equippable);

	UFUNCTION(BlueprintImplementableEvent)
	void SwitchToView(EItTakesXViewType NewItTakesXView);

	void SetCurrentOverlappingVehicleCore(class AVehicleControllerActor* VehicleController);

	UFUNCTION(BlueprintImplementableEvent)
	void AfterDetachFromVehicle();

	void RespawnAtTransform(FTransform RespawnTransform);

	bool IsDead() const;

	UFUNCTION()
	void BeforeDrivingVehicleDestroy();

	class AVehicleControllerActor* GetVehicleControllerFromCharacter();
};
