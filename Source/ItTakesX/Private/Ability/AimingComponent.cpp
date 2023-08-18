// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/AimingComponent.h"

#include "Camera/CameraComponent.h"
#include "Character/ItTakesXCharacter.h"
#include "Interface/Aimable.h"

// Sets default values for this component's properties
UAimingComponent::UAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// FollowCamera = GetOwner()->FindComponentByClass<UCameraComponent>();

	// ...
}


// Called every frame
void UAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AimScene();

	// ...
}

// FVector UAimingComponent::GetFollowCameraLocation() const
// {
// 	if (FollowCamera == nullptr)
// 	{
// 		return FVector::Zero();
// 	}
// 	return FollowCamera->GetComponentLocation();
// }

void UAimingComponent::AimScene()
{
	// GEngine->AddOnScreenDebugMessage(
	// 	-1, 15.f, FColor::Yellow,
	// 	FString::Printf(
	// 		TEXT("AimScene: test test")));

	auto Character = Cast<AItTakesXCharacter>(GetOwner());
	if (Character == nullptr)
	{
		return;
	}

	auto FollowCamera = Character->GetFollowCamera();

	if (FollowCamera == nullptr) { return; }


	FVector Start = Character->GetFollowCameraLocation();
	FVector End = Start + FollowCamera->GetForwardVector() * MaxGrabbingDistance;

	UWorld* World = GetWorld();
	TArray<FHitResult> HitResults;

	
	
	if (World->LineTraceMultiByChannel(HitResults, Start, End, ECollisionChannel::ECC_WorldDynamic))
	{
	}
	CurrentHitResults = HitResults;

	// for (auto Result : CurrentHitResults)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	// 	                                 FString::Printf(
	// 		                                 TEXT("Result actor name %s, component name %s"),
	// 		                                 *Result.GetActor()->GetName(), *Result.Component->GetName()));
	// }

	auto CurrentNearestHitAimable = GetNearestAimingAimable();

	// GEngine->AddOnScreenDebugMessage(
	// 	-1, 15.f, FColor::Yellow,
	// 	FString::Printf(
	// 		TEXT("HitResults length = %d"), HitResults.Num()));
	// GEngine->AddOnScreenDebugMessage(
	// 	-1, 15.f, FColor::Yellow,
	// 	FString::Printf(
	// 		TEXT("CurrentAimingAimable = %p"), CurrentAimingAimable));

	if (CurrentAimingAimable != CurrentNearestHitAimable)
	{
		CurrentAimingAimable = CurrentNearestHitAimable;
		if (CurrentAimingAimable)
		{
			AActor* AimingActor = Cast<AActor>(CurrentAimingAimable);
			if (AimingActor)
			{
				GEngine->AddOnScreenDebugMessage(
					-1, 15.f, FColor::Yellow,
					FString::Printf(
						TEXT("Current aiming actor: %s"), *AimingActor->GetName()));
			}
		}
	}
}

FHitResult* UAimingComponent::GetNearestHitResult() const
{
	float MinDistance = TNumericLimits<float>::Max();
	FHitResult* NearestResult = nullptr;

	for (auto HitResult : CurrentHitResults)
	{
		if (MinDistance > HitResult.Distance)
		{
			MinDistance = HitResult.Distance;
			NearestResult = &HitResult;
		}
	}
	return NearestResult;
}

IAimable* UAimingComponent::GetNearestAimingAimable() const
{
	float MinDistance = TNumericLimits<float>::Max();
	IAimable* NearestAimable = nullptr;

	for (auto HitResult : CurrentHitResults)
	{
		auto Aimable = Cast<IAimable>(HitResult.GetActor());
		// 必须击中 Mesh 才可以
		auto MeshComponent = Cast<UMeshComponent>(HitResult.Component);
		if (Aimable != nullptr && MeshComponent != nullptr && MinDistance > HitResult.Distance)
		{
			MinDistance = HitResult.Distance;
			NearestAimable = Aimable;
		}
	}
	return NearestAimable;
}

IAimable* UAimingComponent::GetCurrentAimingAimable() const
{
	return CurrentAimingAimable;
}
