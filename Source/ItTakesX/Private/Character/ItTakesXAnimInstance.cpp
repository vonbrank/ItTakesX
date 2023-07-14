// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ItTakesXAnimInstance.h"

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

	FRotator AimRotation = ItTakesXCharacter->GetBaseAimRotation();
	FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(ItTakesXCharacter->GetVelocity());
	YawOffset = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;

	// YawOffset = UKismetMathLibrary::MakeRotFromX(UKismetMathLibrary::InverseTransformDirection(ItTakesXCharacter->GetActorTransform(), Velocity)).Yaw;
}
