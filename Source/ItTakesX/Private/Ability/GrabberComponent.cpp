// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GrabberComponent.h"

#include "Ability/AimingComponent.h"
#include "Camera/CameraComponent.h"
#include "Character/ItTakesXCharacter.h"
#include "Interface/Aimable.h"
#include "Interface/Hoistable.h"
#include "Interface/VehicleNode.h"
#include "Kismet/GameplayStatics.h"

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
	AActor* HoistingActor = nullptr;

	if (CurrentHoistingHoistable)
	{
		CurrentHoistingHoistable->OnEndHoisting_Implementation(GetOwner());

		CurrentHoistingHoistable = nullptr;

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

		// GEngine->AddOnScreenDebugMessage(
		// 	-1, 15.f, FColor::Yellow,
		// 	FString::Printf(
		// 		TEXT("Current Hoisting actor: %s"), *HoistingActor->GetName()));
	}

	return true;
}

bool UGrabberComponent::InteractWithHoisting()
{
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
		if (CurrentHoistingHoistable)
		{
			CurrentHoistingHoistable->OnEndHoisting_Implementation(GetOwner());
		}
		CurrentHoistingHoistable = nullptr;
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

	FRotator NewRotation = HoistingActor->GetActorRotation();

	switch (Direction)
	{
	case Direction_Up:
		NewRotation.Pitch += Value;
		break;
	case Direction_Down:
		NewRotation.Pitch -= Value;
		break;
	case Direction_Left:
		NewRotation.Yaw -= Value;
		break;
	case Direction_Right:
		NewRotation.Yaw += Value;
		break;
	default:
		break;
	}

	HoistingActor->SetActorRotation(FMath::RInterpTo(HoistingActor->GetActorRotation(), NewRotation,
	                                                 UGameplayStatics::GetWorldDeltaSeconds(this), 50.f));

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

	GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Cyan,
	                                 FString::Printf(TEXT("CurrentSelectDistance: %f"), CurrentSelectDistance));

	return true;
}
