// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/CombatComponent.h"

#include "Ability/AimingComponent.h"
#include "Ability/InventoryComponent.h"
#include "Environment/BombActor.h"
#include "Interface/VehicleNode.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	AimingComponent = GetOwner()->FindComponentByClass<UAimingComponent>();
	InventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UCombatComponent::InteractWithShoot()
{
	if (AimingComponent == nullptr)
	{
		return false;
	}

	if (InventoryComponent == nullptr || !InventoryComponent->HasWeaponEquipped())
	{
		return false;
	}

	IAimable* Aimable = AimingComponent->GetCurrentAimingAimable();
	ABombActor* BombActor = Cast<ABombActor>(Aimable);

	if (BombActor)
	{
		BombActor->Explode();
	}

	return true;
}

bool UCombatComponent::InteractWithDisconnectVehicleComponent()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(TEXT("InteractWithDisconnectVehicleComponent")));

	if (AimingComponent == nullptr)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(TEXT("AimingComponent is null")));
		return false;
	}

	if (InventoryComponent == nullptr || !InventoryComponent->HasWeaponEquipped())
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(TEXT("InventoryComponent is null or not equip weapon")));
		return false;
	}

	IAimable* Aimable = AimingComponent->GetCurrentAimingAimable();
	IVehicleNode* VehicleNode = Cast<IVehicleNode>(Aimable);
	if (VehicleNode)
	{
		VehicleNode->DetachFromAllAdjacentVehicleNode();

		return true;
	}
	return false;
}
