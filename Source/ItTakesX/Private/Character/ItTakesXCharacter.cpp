// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ItTakesXCharacter.h"

#include "Ability/AimingComponent.h"
#include "Ability/DrivingComponent.h"
#include "Ability/GlidingComponent.h"
#include "Ability/GrabberComponent.h"
#include "Ability/InventoryComponent.h"
#include "Ability/Magnet.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
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

	Inventory = CreateDefaultSubobject<UInventoryComponent>("InventoryComp");

	Driving = CreateDefaultSubobject<UDrivingComponent>("DrivingComp");

	Gliding = CreateDefaultSubobject<UGlidingComponent>("GlidingComp");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AItTakesXCharacter::BeginPlay()
{
	Super::BeginPlay();

	Inventory->OnCurrentEquippableUpdate.AddDynamic(this, &ThisClass::OnCurrentEquippableUpdate);
}

// Called every frame
void AItTakesXCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		GetCharacterMovement()->GravityScale = 1.0;
		GetCharacterMovement()->AirControl = 0.2;
	}
}

// Called to bind functionality to input
void AItTakesXCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Glide"), IE_Pressed, this, &ThisClass::SwitchGliding);
	PlayerInputComponent->BindAction(TEXT("Operate"), IE_Pressed, this, &ThisClass::HandlePressingE);
	PlayerInputComponent->BindAction(TEXT("FAction"), IE_Pressed, this, &ThisClass::HandlePressingF);
	PlayerInputComponent->BindAction(TEXT("ZAction"), IE_Repeat, this, &ThisClass::HandleRepeatingZ);
	PlayerInputComponent->BindAction(TEXT("XAction"), IE_Repeat, this, &ThisClass::HandleRepeatingX);
	PlayerInputComponent->BindAction(TEXT("LShiftAction"), IE_Repeat, this, &ThisClass::HandleRepeatingLShift);
	PlayerInputComponent->BindAction(TEXT("LCtrlAction"), IE_Repeat, this, &ThisClass::HandleRepeatingLCtrl);
	PlayerInputComponent->BindAction(TEXT("ZAction"), IE_Pressed, this, &ThisClass::HandleRepeatingZ);
	PlayerInputComponent->BindAction(TEXT("XAction"), IE_Pressed, this, &ThisClass::HandleRepeatingX);
	PlayerInputComponent->BindAction(TEXT("LShiftAction"), IE_Pressed, this, &ThisClass::HandleRepeatingLShift);
	PlayerInputComponent->BindAction(TEXT("LCtrlAction"), IE_Pressed, this, &ThisClass::HandleRepeatingLCtrl);
	PlayerInputComponent->BindAction(TEXT("OneAction"), IE_Pressed, this, &ThisClass::HandlePressingOne);
	PlayerInputComponent->BindAction(TEXT("TwoAction"), IE_Pressed, this, &ThisClass::HandlePressingTwo);
	PlayerInputComponent->BindAction(TEXT("ThreeAction"), IE_Pressed, this, &ThisClass::HandlePressingThree);

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
	FVehicleCoreCommand NewCommand;
	if (Driving->ExecuteVehicleCommand(NewCommand))
	{
		return;
	}
}

bool AItTakesXCharacter::HasMagnetEquipped() const
{
	return Inventory != nullptr && Inventory->HasMagnetEquipped();
}

void AItTakesXCharacter::PickUpAndEquip(TScriptInterface<IEquippable> Equippable)
{
	Inventory->AddAndEquip(Equippable);
}

void AItTakesXCharacter::OnCurrentEquippableUpdate(TScriptInterface<IEquippable> NewEquippableInterface)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Equip Update")));

	auto NewEquippable = NewEquippableInterface.GetInterface();

	if (NewEquippable == nullptr)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		return;
	}

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = NewEquippable->GetMaxWalkingSpeed();

	auto MagnetToEquip = Cast<AMagnet>(NewEquippable);
	auto GliderToEquip = Cast<AGlider>(NewEquippable);

	if (MagnetToEquip)
	{
		const auto HandSocket = GetMesh()->GetSocketByName(TEXT("hand_rSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(MagnetToEquip, GetMesh());
		}
		MagnetToEquip->SetOwner(this);
	}
	else if (GliderToEquip)
	{
		const auto GliderSocket = GetMesh()->GetSocketByName(TEXT("GliderSocket"));
		if (GliderSocket)
		{
			GliderSocket->AttachActor(GliderToEquip, GetMesh());
			GliderToEquip->SetOwner(this);
		}
		GliderToEquip->SetActorScale3D(FVector(1, 1, 1));
	}

	if (bIsSwitching)
	{
		return;
	}

	if (ItTakesXView == EItTakesXViewType_Normal && NewEquippable->GetViewType() == EItTakesXViewType_NiceToAiming)
	{
		SwitchToView(EItTakesXViewType_NiceToAiming);
	}
	else if (ItTakesXView == EItTakesXViewType_NiceToAiming && NewEquippable->GetViewType() == EItTakesXViewType_Normal)
	{
		SwitchToView(EItTakesXViewType_Normal);
	}
	ItTakesXView = NewEquippable->GetViewType();
}

void AItTakesXCharacter::SetCurrentOverlappingVehicleCore(AVehicleCoreActor* VehicleCoreActor)
{
	if (Driving)
	{
		Driving->SetCurrenOverlappingVehicle(VehicleCoreActor);
	}
}

void AItTakesXCharacter::SwitchGliding()
{
	Gliding->ToggleGliding();
}

bool AItTakesXCharacter::IsGliding() const
{
	return Gliding->IsGliding();
}

void AItTakesXCharacter::HandlePressingOne()
{
	Inventory->SwitchToEquippableByIndex(1);
}

void AItTakesXCharacter::HandlePressingTwo()
{
	Inventory->SwitchToEquippableByIndex(2);
}

void AItTakesXCharacter::HandlePressingThree()
{
	Inventory->SwitchToEquippableByIndex(3);
}
