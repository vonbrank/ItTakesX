// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ItTakesXCharacter.h"

#include "Ability/AimingComponent.h"
#include "Ability/GrabberComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AItTakesXCharacter::AItTakesXCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Aiming = CreateDefaultSubobject<UAimingComponent>("AimingComp");

	Grabber = CreateDefaultSubobject<UGrabberComponent>("GrabberComp");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AItTakesXCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItTakesXCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AItTakesXCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Operate"), IE_Pressed, this, &ThisClass::HandlePressingE);
	PlayerInputComponent->BindAction(TEXT("FAction"), IE_Pressed, this, &ThisClass::HandlePressingF);
	PlayerInputComponent->BindAction(TEXT("ZAction"), IE_Repeat, this, &ThisClass::HandleRepeatingZ);
	PlayerInputComponent->BindAction(TEXT("XAction"), IE_Repeat, this, &ThisClass::HandleRepeatingX);
	PlayerInputComponent->BindAction(TEXT("LShiftAction"), IE_Repeat, this, &ThisClass::HandleRepeatingLShift);
	PlayerInputComponent->BindAction(TEXT("LCtrlAction"), IE_Repeat, this, &ThisClass::HandleRepeatingLCtrl);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ThisClass::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ThisClass::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &ThisClass::Zoom);
}

void AItTakesXCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void AItTakesXCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void AItTakesXCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AItTakesXCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AItTakesXCharacter::Zoom(float Value)
{
	Grabber->InteractWithZoomingHoistable(Value * 100);
}

void AItTakesXCharacter::HandlePressingE()
{
	if (Grabber->ToggleHoistingActor())
	{
		return;
	}
}

void AItTakesXCharacter::HandleRepeatingLShift()
{
	Grabber->InteractWithHoistingObjectRotation(ERotateDirection::Direction_Up, 10.f);
}

void AItTakesXCharacter::HandleRepeatingLCtrl()
{
	Grabber->InteractWithHoistingObjectRotation(ERotateDirection::Direction_Down, 10.f);
}

void AItTakesXCharacter::HandleRepeatingZ()
{
	Grabber->InteractWithHoistingObjectRotation(ERotateDirection::Direction_Left, 10.f);
}

void AItTakesXCharacter::HandleRepeatingX()
{
	Grabber->InteractWithHoistingObjectRotation(ERotateDirection::Direction_Right, 10.f);
}

FVector AItTakesXCharacter::GetFollowCameraLocation() const
{
	if (FollowCamera == nullptr)
	{
		return FVector::Zero();
	}
	return FollowCamera->GetComponentLocation();
}

void AItTakesXCharacter::HandlePressingF()
{
	if (Grabber->InteractWithComposing())
	{
		return;
	}
}
