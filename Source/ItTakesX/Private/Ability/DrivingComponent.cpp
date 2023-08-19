// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/DrivingComponent.h"

#include "BuildingSystem/VehicleControllerActor.h"
#include "BuildingSystem/Component/VehicleComponentThruster.h"
#include "BuildingSystem/Core/VehicleCoreFloatingPanel.h"
#include "BuildingSystem/Core/VehicleCoreHoveringCar.h"
#include "BuildingSystem/Core/VehicleCoreQuadcopter.h"
#include "BuildingSystem/Core/VehicleCoreSimpleAirplane.h"
#include "Character/ItTakesXCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values for this component's properties
UDrivingComponent::UDrivingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDrivingComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AItTakesXCharacter>(GetOwner());
	if (Character)
	{
		PlayerControllerRef = Cast<APlayerController>(Character->GetController());
	}
}


// Called every frame
void UDrivingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentDrivingVehicle && CurrentDrivingVehicle->IsVehicleStartup() && Character)
	{
		FRotator NewRotation = Character->Controller->GetControlRotation();
		if (CurrentDrivingVehicle->IsAimingOpenFireMode())
		{
			NewRotation.Pitch = CurrentControllerPitch;

			if (PlayerControllerRef)
			{
				FHitResult HitResult;
				if (PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult))
				{
					// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 25.f, 12, FColor::Red, false, -1.f);
					if (CurrentDrivingVehicle && !(Cast<IVehicleNode>(HitResult.GetActor()) || Cast<AItTakesXCharacter>(
						HitResult.GetActor())))
					{
						CurrentDrivingVehicle->TurretLookAtTarget(HitResult.Location);
					}
				}
			}
		}
		NewRotation.Yaw = CurrentDrivingVehicle->GetActorRotation().Yaw;
		Character->Controller->SetControlRotation(NewRotation);
	}
	else
	{
	}
}

void UDrivingComponent::SetCurrenOverlappingVehicle(AVehicleControllerActor* NewOverlappingVehicle)
{
	CurrentOverlappingVehicle = NewOverlappingVehicle;
}

bool UDrivingComponent::InteractWithMoveForward(float Value)
{
	if (CurrentDrivingVehicle == nullptr)
	{
		return false;
	}

	if (!CurrentDrivingVehicle->IsVehicleStartup())
	{
		return false;
	}

	AVehicleCoreHoveringCar* VehicleCoreHoveringCar = Cast<AVehicleCoreHoveringCar>(CurrentDrivingVehicle);
	if (VehicleCoreHoveringCar)
	{
		VehicleCoreHoveringCar->HandleControl(EHoveringCarControl_MoveForward, Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::InteractWithTurnRight(float Value)
{
	if (CurrentDrivingVehicle == nullptr)
	{
		return false;
	}
	if (!CurrentDrivingVehicle->IsVehicleStartup())
	{
		return false;
	}

	AVehicleCoreHoveringCar* VehicleCoreHoveringCar = Cast<AVehicleCoreHoveringCar>(CurrentDrivingVehicle);
	if (VehicleCoreHoveringCar)
	{
		VehicleCoreHoveringCar->HandleControl(EHoveringCarControl_TurnRight, Value);
		return true;
	}
	return false;
}

bool UDrivingComponent::InteractWithPitchUp(float Value)
{
	if (CurrentDrivingVehicle == nullptr)
	{
		return false;
	}
	return false;
}

bool UDrivingComponent::ToggleVehicle(bool& bOutShouldActiveMovement)
{
	bOutShouldActiveMovement = false;

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("start ToggleVehicle")
	//                                  ));

	if (CurrentDrivingVehicle)
	{
		bool bShutdownResult = CurrentDrivingVehicle->ShutdownVehicle();
		if (Character)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			//                                  FString::Printf(
			// 	                                 TEXT("location of character %s"),
			// 	                                 *Character->GetActorLocation().ToString()));

			CurrentDrivingVehicle->DetachCurrentCharacter();
			bOutShouldActiveMovement = true;

			// Character->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			// Character->SetActorLocation(CurrentOverlappingVehicle->GetActorLocation() + FVector::UpVector * 100);
			// Character->GetMovementComponent()->Velocity = FVector::Zero();
			CurrentDrivingVehicle = nullptr;
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			//                                  FString::Printf(
			// 	                                 TEXT("location of character %s"),
			// 	                                 *Character->GetActorLocation().ToString()));
			// Character->();
			Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
		return bShutdownResult;
	}
	if (CurrentOverlappingVehicle == nullptr)
	{
		return false;
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("start ToggleVehicle 2")
	//                                  ));
	CurrentDrivingVehicle = CurrentOverlappingVehicle;
	CurrentDrivingVehicle->AttachCharacter(Character);
	CurrentDrivingVehicle->VehicleDestroyDelegate.AddDynamic(
		Character, &AItTakesXCharacter::BeforeDrivingVehicleDestroy);

	CurrentControllerPitch = Character->Controller->GetControlRotation().Pitch;
	Character->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	return CurrentDrivingVehicle->StartupVehicle(true);
}

bool UDrivingComponent::Throttle(float Value)
{
	if (CurrentDrivingVehicle == nullptr || !CurrentDrivingVehicle->IsVehicleStartup())
	{
		return false;
	}
	//
	// AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
	// if (VehicleCoreQuadcopter)
	// {
	// 	VehicleCoreQuadcopter->Throttle(Value);
	// 	return true;
	// }
	//
	// AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
	// if (VehicleCoreSimpleAirplane)
	// {
	// 	VehicleCoreSimpleAirplane->Throttle(Value);
	// 	return true;
	// }
	//
	// AVehicleCoreFloatingPanel* VehicleCoreFloatingPanel = Cast<AVehicleCoreFloatingPanel>(CurrentOverlappingVehicle);
	// if (VehicleCoreFloatingPanel)
	// {
	// 	VehicleCoreFloatingPanel->Throttle(Value);
	// 	return true;
	// }

	if (CurrentDrivingVehicle)
	{
		CurrentDrivingVehicle->Throttle(Value);
		return true;
	}

	return false;
}

// bool UDrivingComponent::Pitch(float Value)
// {
// 	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
// 	{
// 		return false;
// 	}
//
// 	AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
// 	if (VehicleCoreQuadcopter)
// 	{
// 		VehicleCoreQuadcopter->Pitch(Value);
// 		return true;
// 	}
//
// 	AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
// 	if (VehicleCoreSimpleAirplane)
// 	{
// 		VehicleCoreSimpleAirplane->Pitch(Value);
// 		return true;
// 	}
//
// 	return false;
// }
//
// bool UDrivingComponent::Roll(float Value)
// {
// 	if (CurrentOverlappingVehicle == nullptr || !CurrentOverlappingVehicle->IsVehicleStartup())
// 	{
// 		return false;
// 	}
//
// 	AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
// 	if (VehicleCoreQuadcopter)
// 	{
// 		VehicleCoreQuadcopter->Roll(Value);
// 		return true;
// 	}
//
// 	AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
// 	if (VehicleCoreSimpleAirplane)
// 	{
// 		VehicleCoreSimpleAirplane->Roll(Value);
// 		return true;
// 	}
//
// 	return false;
// }

bool UDrivingComponent::Turn(float Value)
{
	if (CurrentDrivingVehicle == nullptr || !CurrentDrivingVehicle->IsVehicleStartup())
	{
		return false;
	}
	//
	// AVehicleCoreQuadcopter* VehicleCoreQuadcopter = Cast<AVehicleCoreQuadcopter>(CurrentOverlappingVehicle);
	// if (VehicleCoreQuadcopter)
	// {
	// 	VehicleCoreQuadcopter->Yaw(Value);
	// 	return true;
	// }
	//
	// AVehicleCoreSimpleAirplane* VehicleCoreSimpleAirplane = Cast<AVehicleCoreSimpleAirplane>(CurrentOverlappingVehicle);
	// if (VehicleCoreSimpleAirplane)
	// {
	// 	VehicleCoreSimpleAirplane->Yaw(Value);
	// 	return true;
	// }
	//
	// AVehicleCoreFloatingPanel* VehicleCoreFloatingPanel = Cast<AVehicleCoreFloatingPanel>(CurrentOverlappingVehicle);
	// if (VehicleCoreFloatingPanel)
	// {
	// 	VehicleCoreFloatingPanel->Turn(Value);
	// 	return true;
	// }

	if (CurrentDrivingVehicle)
	{
		CurrentDrivingVehicle->Turn(Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::AircraftThrottle(float Value)
{
	if (CurrentDrivingVehicle == nullptr || !CurrentDrivingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentDrivingVehicle)
	{
		CurrentDrivingVehicle->AircraftThrottle(Value * 10);
		return true;
	}

	return false;
}

bool UDrivingComponent::AircraftTurn(float Value)
{
	if (CurrentDrivingVehicle == nullptr || !CurrentDrivingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentDrivingVehicle)
	{
		CurrentDrivingVehicle->AircraftTurn(Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::AircraftPitch(float Value)
{
	if (CurrentDrivingVehicle == nullptr || !CurrentDrivingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentDrivingVehicle)
	{
		CurrentDrivingVehicle->AircraftPitch(Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::ToggleOpenFire()
{
	if (CurrentDrivingVehicle == nullptr || !CurrentDrivingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentDrivingVehicle)
	{
		CurrentDrivingVehicle->ToggleOpenFire();
		return true;
	}

	return false;
}

bool UDrivingComponent::LaunchProjectile()
{
	if (CurrentDrivingVehicle == nullptr || !CurrentDrivingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentDrivingVehicle)
	{
		CurrentDrivingVehicle->LaunchProjectile();
		return true;
	}

	return false;
}

bool UDrivingComponent::VerticalRotateTurret(float Value)
{
	if (CurrentDrivingVehicle == nullptr || !CurrentDrivingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentDrivingVehicle)
	{
		CurrentDrivingVehicle->VerticalRotateTurret(Value);
		return true;
	}

	return false;
}

bool UDrivingComponent::HorizontalRotateTurret(float Value)
{
	if (CurrentDrivingVehicle == nullptr || !CurrentDrivingVehicle->IsVehicleStartup())
	{
		return false;
	}

	if (CurrentDrivingVehicle)
	{
		CurrentDrivingVehicle->HorizontalRotateTurret(Value);
		return true;
	}

	return false;
}

float UDrivingComponent::GetDrivingVehicleHealth()
{
	if (CurrentDrivingVehicle)
	{
		return CurrentDrivingVehicle->GetHealth();
	}
	return 0;
}

float UDrivingComponent::GetDrivingVehicleMaxHealth()
{
	if (CurrentDrivingVehicle)
	{
		return CurrentDrivingVehicle->GetMaxHealth();
	}
	return 0;
}

float UDrivingComponent::GetDrivingVehicleHealthPercentage()
{
	if (CurrentDrivingVehicle)
	{
		return FMath::Max(0, CurrentDrivingVehicle->GetHealth()) / CurrentDrivingVehicle->GetMaxHealth();
	}
	return 0.5;
}

float UDrivingComponent::GetDrivingVehicleArmourHealth()
{
	if (CurrentDrivingVehicle)
	{
		return CurrentDrivingVehicle->GetArmourHealth();
	}
	return 0;
}

float UDrivingComponent::GetDrivingVehicleArmourMaxHealth()
{
	if (CurrentDrivingVehicle)
	{
		return CurrentDrivingVehicle->GetArmourMaxHealth();
	}
	return 0;
}

float UDrivingComponent::GetDrivingVehicleArmourHealthPercentage()
{
	if (CurrentDrivingVehicle)
	{
		return FMath::Max(0, CurrentDrivingVehicle->GetArmourHealth()) / CurrentDrivingVehicle->GetArmourMaxHealth();
	}
	return 0.5;
}

bool UDrivingComponent::ToggleVehicleAimingMode()
{
	if (CurrentDrivingVehicle)
	{
		if (CurrentDrivingVehicle->IsAimingOpenFireMode())
		{
			if (PlayerControllerRef)
			{
				PlayerControllerRef->bShowMouseCursor = false;
			}
			CurrentDrivingVehicle->SetAimingOpenFireMode(false);
		}
		else
		{
			if (Character)
			{
				CurrentControllerPitch = Character->Controller->GetControlRotation().Pitch;
			}
			if (PlayerControllerRef)
			{
				PlayerControllerRef->bShowMouseCursor = true;
			}
			CurrentDrivingVehicle->SetAimingOpenFireMode(true);
		}


		return true;
	}
	return false;
}

bool UDrivingComponent::IsOverlappingVehicle()
{
	return CurrentOverlappingVehicle != nullptr;
}

FVector UDrivingComponent::GetLeftHandleLocation()
{
	if (CurrentDrivingVehicle)
	{
		return CurrentDrivingVehicle->GetLeftHandleLocation();
	}
	return FVector::Zero();
}

FVector UDrivingComponent::GetRightHandleLocation()
{
	if (CurrentDrivingVehicle)
	{
		return CurrentDrivingVehicle->GetRightHandleLocation();
	}
	return FVector::Zero();
}
