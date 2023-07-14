// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GrabberComponent.h"

#include "Ability/AimingComponent.h"
#include "Ability/InventoryComponent.h"
#include "Ability/Magnet.h"
#include "Camera/CameraComponent.h"
#include "Character/ItTakesXCharacter.h"
#include "Effect/DottedLazer.h"
#include "Interface/Aimable.h"
#include "Interface/Hoistable.h"
#include "Interface/VehicleNode.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UGrabberComponent::UGrabberComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabberComponent::BeginPlay()
{
	Super::BeginPlay();

	AimingComponent = GetOwner()->FindComponentByClass<UAimingComponent>();
	InventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
}


// Called every frame
void UGrabberComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	InteractWithHoisting();
}

bool UGrabberComponent::ToggleHoistingActor()
{
	if (!(InventoryComponent != nullptr && InventoryComponent->HasMagnetEquipped()))
	{
		return false;
	}

	AActor* HoistingActor = nullptr;
	auto Magnet = Cast<AMagnet>(InventoryComponent->GetCurrentEquippable());

	if (CurrentHoistingHoistable)
	{
		CurrentHoistingHoistable->OnEndHoisting_Implementation(GetOwner());

		CurrentHoistingHoistable = nullptr;

		if (Magnet)
		{
			Magnet->DestroyCurrentMagnetEffect();
		}

		// TODO other action for dropdown;

		return true;
	}

	if (AimingComponent == nullptr) return false;

	IAimable* Aimable = AimingComponent->GetCurrentAimingAimable();
	if (Aimable == nullptr) return false;

	IHoistable* Hoistable = Cast<IHoistable>(Aimable);
	if (Hoistable == nullptr) return false;

	CurrentHoistingHoistable = Hoistable;

	HoistingActor = Cast<AActor>(CurrentHoistingHoistable);
	if (HoistingActor)
	{
		auto Character = Cast<AItTakesXCharacter>(GetOwner());
		FVector StartLocation = Character == nullptr
			                        ? GetOwner()->GetActorLocation()
			                        : Character->GetFollowCameraLocation();
		CurrentSelectDistance = (StartLocation - HoistingActor->GetActorLocation()).Length();
		CurrentHoistingHoistable->OnBeginHoisting_Implementation(GetOwner());
		if (Magnet)
		{
			CurrentMagnetEffect = Magnet->SpawnNewMagnetEffect();
		}

		// GEngine->AddOnScreenDebugMessage(
		// 	-1, 15.f, FColor::Yellow,
		// 	FString::Printf(
		// 		TEXT("Current Hoisting actor: %s"), *HoistingActor->GetName()));
	}

	return true;
}

bool UGrabberComponent::InteractWithHoisting()
{
	if (!(InventoryComponent && InventoryComponent->HasMagnetEquipped()))
	{
		return false;
	}

	if (CurrentHoistingHoistable)
	{
		auto Character = Cast<AItTakesXCharacter>(GetOwner());
		if (Character == nullptr)
		{
			return false;
		}

		auto HoistingActor = Cast<AActor>(CurrentHoistingHoistable);
		if (HoistingActor == nullptr)
		{
			return false;
		}
		auto FollowCamera = Character->GetFollowCamera();
		if (FollowCamera == nullptr)
		{
			return false;
		}

		FVector Start = Character->GetFollowCameraLocation();
		FVector End = Start + FollowCamera->GetForwardVector() * CurrentSelectDistance;
		HoistingActor->SetActorLocation(FMath::VInterpTo(HoistingActor->GetActorLocation(), End,
		                                                 UGameplayStatics::GetWorldDeltaSeconds(this), 5.f));
		//
		if (CurrentMagnetEffect)
		{
			CurrentMagnetEffect->SetEndLocation(FMath::VInterpTo(HoistingActor->GetActorLocation(), End,
			                                                     UGameplayStatics::GetWorldDeltaSeconds(this), 5.f));
		}
		//
		// GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan,
		//                                  FString::Printf(TEXT("CurrentSelectDistance = %f"), CurrentSelectDistance));

		return true;
	}

	return false;
}

bool UGrabberComponent::InteractWithComposing()
{
	auto VehicleNode = Cast<IVehicleNode>(CurrentHoistingHoistable);
	if (VehicleNode == nullptr) return false;

	bool bAttachingResult = VehicleNode->AttachToCurrentOverlappingVehicleNode();

	if (bAttachingResult)
	{
		ToggleHoistingActor();
	}
	return bAttachingResult;
}

bool UGrabberComponent::InteractWithHoistingObjectRotation(ERotateDirection Direction, float Value)
{
	auto HoistingActor = Cast<AActor>(CurrentHoistingHoistable);
	if (HoistingActor == nullptr)
	{
		return false;
	}
	Value *= GetWorld()->DeltaTimeSeconds;

	FVector N;

	switch (Direction)
	{
	case Direction_Up:
		N = GetHoistingActorHorizontalRotatingAxisRight();
		Value = -Value;
		break;
	case Direction_Down:
		N = GetHoistingActorHorizontalRotatingAxisRight();
		break;
	case Direction_Left:
		N = FVector::UpVector;
		break;
	case Direction_Right:
		N = FVector::UpVector;
		Value = -Value;
		break;
	default:
		N = FVector::UpVector;
		break;
	}

	FQuat Rotation = FQuat(N, Value);
	FQuat CurrentRotation = HoistingActor->GetActorRotation().Quaternion();
	FQuat NewRotation = Rotation * CurrentRotation;
	HoistingActor->SetActorRotation(NewRotation);

	// DrawDebugLine(GetWorld(), HoistingActor->GetActorLocation(),
	//               HoistingActor->GetActorLocation() + HoistingActor->GetActorForwardVector() * 1000,
	//               FColor::Red, false, 5);

	// FRotator NewRotation = HoistingActor->GetActorRotation();


	// HoistingActor->SetActorRotation(FMath::RInterpTo(HoistingActor->GetActorRotation(), NewRotation,
	//                                                  UGameplayStatics::GetWorldDeltaSeconds(this), 50.f));

	return true;
}

bool UGrabberComponent::InteractWithZoomingHoistable(float Value)
{
	if (CurrentHoistingHoistable == nullptr)
	{
		return false;
	}

	CurrentSelectDistance += Value;
	CurrentSelectDistance = FMath::Clamp(CurrentSelectDistance, 1000.f, 10000.f);

	// GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan,
	//                                  FString::Printf(TEXT("CurrentSelectDistance: %f"), CurrentSelectDistance));

	return true;
}

FVector UGrabberComponent::GetHoistingActorHorizontalRotatingAxisRight()
{
	auto Character = Cast<AItTakesXCharacter>(GetOwner());
	if (Character == nullptr) { return FVector::RightVector; }

	auto FollowCamera = Character->GetFollowCamera();
	if (FollowCamera == nullptr) { return FVector::RightVector; }

	FVector Res = FVector::CrossProduct(FollowCamera->GetForwardVector(), FVector::UpVector);
	Res.Normalize();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Res = %s"), *Res.ToString()));

	return Res;
}
