// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ItTakesXCharacter.h"

#include "Ability/AimingComponent.h"
#include "Ability/CombatComponent.h"
#include "Ability/DrivingComponent.h"
#include "Ability/GlidingComponent.h"
#include "Ability/GrabberComponent.h"
#include "Ability/HealthComponent.h"
#include "Ability/InventoryComponent.h"
#include "Ability/Magnet.h"
#include "Ability/Weapon/Weapon.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameModes/ItTakesXGameMode.h"
#include "Kismet/GameplayStatics.h"

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

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");

	Driving = CreateDefaultSubobject<UDrivingComponent>("DrivingComp");

	Gliding = CreateDefaultSubobject<UGlidingComponent>("GlidingComp");

	Combat = CreateDefaultSubobject<UCombatComponent>("CombatComp");

	Health = CreateDefaultSubobject<UHealthComponent>("HealthComp");

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AItTakesXCharacter::BeginPlay()
{
	Super::BeginPlay();

	InventoryComponent->OnCurrentEquippableUpdate.AddDynamic(this, &ThisClass::OnCurrentEquippableUpdate);
	ItTakesXGameMode = Cast<AItTakesXGameMode>(UGameplayStatics::GetGameMode(this));
	if (ItTakesXGameMode)
	{
		ItTakesXGameMode->SetCharacterBeginTransform(this, GetTransform());
	}
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
	PlayerInputComponent->BindAction(TEXT("KAction"), IE_Pressed, this, &ThisClass::HandlePressingK);
	PlayerInputComponent->BindAction(TEXT("LAction"), IE_Pressed, this, &ThisClass::HandlePressingL);
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
	PlayerInputComponent->BindAction(TEXT("LMBAction"), IE_Pressed, this, &ThisClass::HandlePressingLMB);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ThisClass::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ThisClass::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ThisClass::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ThisClass::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Zoom"), this, &ThisClass::Zoom);
	PlayerInputComponent->BindAxis(TEXT("Throttle"), this, &ThisClass::Throttle);
	// PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &ThisClass::Pitch);
	// PlayerInputComponent->BindAxis(TEXT("Roll"), this, &ThisClass::Roll);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &ThisClass::VehicleTurn);
	PlayerInputComponent->BindAxis(TEXT("AircraftThrottle"), this, &ThisClass::AircraftThrottle);
	PlayerInputComponent->BindAxis(TEXT("AircraftTurn"), this, &ThisClass::AircraftTurn);
	PlayerInputComponent->BindAxis(TEXT("AircraftTurnPitch"), this, &ThisClass::AircraftPitch);
}

void AItTakesXCharacter::MoveForward(float Value)
{
	if (Driving->InteractWithMoveForward(Value))
	{
		return;
	}

	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Direction(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void AItTakesXCharacter::MoveRight(float Value)
{
	if (Driving->InteractWithTurnRight(Value))
	{
		return;
	}

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
		if (Grabber->IsHoisting())
		{
			SwitchToView(EItTakesXViewType_NiceToGrabbing);
		}
		else
		{
			SwitchToView(EItTakesXViewType_NiceToAiming);
		}

		return;
	}
	if (Combat->InteractWithDisconnectVehicleComponent())
	{
		return;
	}
}

void AItTakesXCharacter::HandleRepeatingLShift()
{
	if (Grabber->InteractWithHoistingObjectRotation(ERotateDirection::Direction_Up, 10.f))
	{
		return;
	}
	if (Driving->VerticalRotateTurret(1))
	{
		return;
	}
}

void AItTakesXCharacter::HandleRepeatingLCtrl()
{
	if (Grabber->InteractWithHoistingObjectRotation(ERotateDirection::Direction_Down, 10.f))
	{
		return;
	}
	if (Driving->VerticalRotateTurret(-1))
	{
		return;
	}
}

void AItTakesXCharacter::HandleRepeatingZ()
{
	if (Grabber->InteractWithHoistingObjectRotation(ERotateDirection::Direction_Left, 10.f))
	{
		return;
	}
	if (Driving->HorizontalRotateTurret(1))
	{
		return;
	}
}

void AItTakesXCharacter::HandleRepeatingX()
{
	if (Grabber->InteractWithHoistingObjectRotation(ERotateDirection::Direction_Right, 10.f))
	{
		return;
	}

	if (Driving->HorizontalRotateTurret(-1))
	{
		return;
	}
}

void AItTakesXCharacter::BeforeDrivingVehicleDestroy()
{
	ToggleDriving();
}

AVehicleControllerActor* AItTakesXCharacter::GetVehicleControllerFromCharacter()
{
	return Driving->GetCurrentDrivingVehicle();
}

bool AItTakesXCharacter::ToggleDriving()
{
	bool bOutShouldActiveMovement = false;
	bool bResult = Driving->ToggleVehicle(bOutShouldActiveMovement);

	if (bOutShouldActiveMovement)
	{
		AfterDetachFromVehicle();
	}

	return bResult;
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
		SwitchToView(EItTakesXViewType_NiceToAiming);
		return;
	}
	if (ToggleDriving())
	{
		return;
	}
}

bool AItTakesXCharacter::HasMagnetEquipped() const
{
	return InventoryComponent != nullptr && InventoryComponent->HasMagnetEquipped();
}

bool AItTakesXCharacter::HasWeaponEquipped() const
{
	return InventoryComponent != nullptr && InventoryComponent->HasWeaponEquipped();
}


void AItTakesXCharacter::PickUpAndEquip(TScriptInterface<IEquippable> Equippable)
{
	InventoryComponent->AddAndEquip(Equippable);
}

void AItTakesXCharacter::OnCurrentEquippableUpdate(TScriptInterface<IEquippable> NewEquippableInterface)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("start on Equip Update")));

	auto NewEquippable = NewEquippableInterface.GetInterface();

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("curren equippable before gliding check: %p"),
	// 	                                 Inventory->GetCurrentEquippable()));

	if (Cast<AGlider>(NewEquippable) == nullptr && IsGliding())
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("toggle after gliding")));
		Gliding->ToggleGliding();
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("curren equippable after gliding check: %p"),
	// 	                                 Inventory->GetCurrentEquippable()));

	if (NewEquippable == nullptr)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("reset animation")));

		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
		SwitchToView(EItTakesXViewType_Normal);
		ItTakesXView = EItTakesXViewType_Normal;
		return;
	}

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = NewEquippable->GetMaxWalkingSpeed();

	auto MagnetToEquip = Cast<AMagnet>(NewEquippable);
	auto GliderToEquip = Cast<AGlider>(NewEquippable);
	auto WeaponToEquip = Cast<AWeapon>(NewEquippable);

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("curren equippable before attaching check: %p"),
	// 	                                 Inventory->GetCurrentEquippable()));

	if (MagnetToEquip)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Equip MagnetToEquip")));
		const auto HandSocket = GetMesh()->GetSocketByName(TEXT("MagnetSocket"));
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
		}
		GliderToEquip->SetOwner(this);
		// GliderToEquip->SetActorScale3D(FVector(1, 1, 1));
	}
	else if (WeaponToEquip)
	{
		const auto WeaponSocket = GetMesh()->GetSocketByName(TEXT("RifleSocket"));
		if (WeaponSocket)
		{
			WeaponSocket->AttachActor(WeaponToEquip, GetMesh());
		}
		WeaponToEquip->SetOwner(this);
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("curren equippable after attaching check: %p"),
	// 	                                 Inventory->GetCurrentEquippable()));

	if (bIsSwitching)
	{
		return;
	}

	SwitchToView(NewEquippable->GetViewType());
	ItTakesXView = NewEquippable->GetViewType();
}

void AItTakesXCharacter::SetCurrentOverlappingVehicleCore(AVehicleControllerActor* VehicleController)
{
	if (Driving)
	{
		Driving->SetCurrenOverlappingVehicle(VehicleController);
	}
}

void AItTakesXCharacter::RespawnAtTransform(FTransform RespawnTransform)
{
	SetActorTransform(RespawnTransform);
	Health->ResetHealth();
}

bool AItTakesXCharacter::IsDead() const
{
	return Health->GetHealth() <= 0;
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
	InventoryComponent->SwitchToEquippableByIndex(1);
}

void AItTakesXCharacter::HandlePressingTwo()
{
	InventoryComponent->SwitchToEquippableByIndex(2);
}

void AItTakesXCharacter::HandlePressingThree()
{
	InventoryComponent->SwitchToEquippableByIndex(3);
}

void AItTakesXCharacter::HandlePressingLMB()
{
	if (Combat->InteractWithShoot())
	{
		return;
	}
}

void AItTakesXCharacter::HandlePressingK()
{
	if (Driving->ToggleOpenFire())
	{
		return;
	}
}

void AItTakesXCharacter::HandlePressingL()
{
	if (Driving->LaunchProjectile())
	{
		return;
	}
}

void AItTakesXCharacter::Throttle(float Value)
{
	if (Driving->Throttle(Value))
	{
		return;
	}
}

// void AItTakesXCharacter::Pitch(float Value)
// {
// 	if (Driving->Pitch(Value))
// 	{
// 		return;
// 	}
// }
//
// void AItTakesXCharacter::Roll(float Value)
// {
// 	if (Driving->Roll(Value))
// 	{
// 		return;
// 	}
// }

void AItTakesXCharacter::VehicleTurn(float Value)
{
	if (Driving->Turn(Value))
	{
		return;
	}
}

void AItTakesXCharacter::AircraftThrottle(float Value)
{
	if (Driving->AircraftThrottle(Value))
	{
		return;
	}
}

void AItTakesXCharacter::AircraftTurn(float Value)
{
	if (Driving->AircraftTurn(Value))
	{
		return;
	}
}

void AItTakesXCharacter::AircraftPitch(float Value)
{
	if (Driving->AircraftPitch(Value))
	{
		return;
	}
}
