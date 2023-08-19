// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ItTakesXAnimInstance.h"

#include "BuildingSystem/VehicleControllerActor.h"
#include "Character/ItTakesXCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UItTakesXAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ItTakesXCharacter = Cast<AItTakesXCharacter>(TryGetPawnOwner());
}

void UItTakesXAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ItTakesXCharacter == nullptr)
	{
		ItTakesXCharacter = Cast<AItTakesXCharacter>(TryGetPawnOwner());
	}
	if (ItTakesXCharacter == nullptr) return;

	FVector Velocity = ItTakesXCharacter->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();

	bIsInAir = ItTakesXCharacter->GetCharacterMovement()->IsFalling();

	bIsAccelerating = ItTakesXCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f;

	bEquippingMagnet = ItTakesXCharacter->HasMagnetEquipped();
	bEquippingWeapon = ItTakesXCharacter->HasWeaponEquipped();
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(TEXT("bEquippingMagnet = %d"), bEquippingMagnet));

	bIsGliding = ItTakesXCharacter->IsGliding();

	FRotator AimRotation = ItTakesXCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ItTakesXCharacter->GetVelocity());
	YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

	bIsDead = ItTakesXCharacter->IsDead();

	auto VehicleController = ItTakesXCharacter->GetVehicleControllerFromCharacter();
	bIsDriving = VehicleController != nullptr;
	if (bIsDriving)
	{
		// auto LeftHandLocation = GetOwningComponent()->GetSocketLocation(FName(TEXT("hand_l")));
		// auto RightHandLocation = GetOwningComponent()->GetSocketLocation(FName(TEXT("hand_r")));
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(
		// 	                                 TEXT("LeftHandLocation = %s, RightHandLocation = %s"),
		// 	                                 *LeftHandLocation.ToString(), *RightHandLocation.ToString()));
		//
		// auto TempTransform = ItTakesXCharacter->GetTransform();
		// TempTransform.SetLocation(FVector::Zero());

		LeftHandOffset = VehicleController->GetLeftHandleLocation();
		RightHandOffset = VehicleController->GetRightHandleLocation();
	}

	// YawOffset = UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::InverseTransformDirection(ItTakesXCharacter->GetActorTransform(), Velocity)).Yaw;
}
