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

		HoistingActor = Cast<AActor>(CurrentHoistingHoistable);
		if (HoistingActor)
		{
			auto RootComponent = Cast<UStaticMeshComponent>(HoistingActor->GetRootComponent());
			RootComponent->SetSimulatePhysics(true);
		}

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
		auto RootComponent = Cast<UStaticMeshComponent>(HoistingActor->GetRootComponent());
		RootComponent->SetSimulatePhysics(false);

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

bool UGrabberComponent::InterfaceWithComposing()
{
	auto VehicleNode = Cast<IVehicleNode>(CurrentHoistingHoistable);
	if (VehicleNode == nullptr) return false;

	bool bAttachingResult = VehicleNode->AttachToCurrentOverlappingVehicleNode();
	CurrentHoistingHoistable = nullptr;
	return bAttachingResult;
}
