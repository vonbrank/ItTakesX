// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/InventoryComponent.h"

#include "Ability/Magnet.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	bHaveUnArm = true;
	bHaveGlider = true;
	bHaveMagnet = false;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddAndEquip(TScriptInterface<IEquippable> NewEquippableInterface)
{
	if (NewEquippableInterface.GetInterface() == CurrentEquippableInterface.GetInterface())
	{
		return;
	}

	if (Cast<AMagnet>(NewEquippableInterface.GetInterface()))
	{
		bHaveMagnet = true;
	}

	UnEquipCurrenEquippable();
	CurrentEquippableInterface = NewEquippableInterface;
	OnCurrentEquippableUpdate.Broadcast(NewEquippableInterface);
}

// bool UInventoryComponent::HasMagnetEquipped() const
// {
// 	return Cast<AMagnet>(GetCurrentEquippable()) != nullptr;
// }
bool UInventoryComponent::SwitchToEquippableByIndex(int Index)
{
	bool bSwitchResult = true;

	TScriptInterface<IEquippable> NewEquippableInterface;
	NewEquippableInterface.SetObject(nullptr);
	NewEquippableInterface.SetInterface(nullptr);

	switch (Index)
	{
	case 1:
		if (bHaveUnArm)
		{
			NewEquippableInterface.SetObject(nullptr);
			NewEquippableInterface.SetInterface(nullptr);
		}
		else
		{
			return false;
		}
		break;
	case 2:
		if (bHaveMagnet)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("new magnet")));

			AMagnet* NewMagnet = GetWorld()->SpawnActor<AMagnet>(MagnetClass);
			NewEquippableInterface.SetObject(NewMagnet);
			NewEquippableInterface.SetInterface(NewMagnet);
		}
		else
		{
			return false;
		}
		break;
	case 3:
		if (bHaveGlider)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("new glider")));

			AGlider* NewGlider = GetWorld()->SpawnActor<AGlider>(GliderClass);
			NewEquippableInterface.SetObject(NewGlider);
			NewEquippableInterface.SetInterface(NewGlider);
		}
		else
		{
			return false;
		}
		break;
	default:
		NewEquippableInterface.SetObject(nullptr);
		NewEquippableInterface.SetInterface(nullptr);
		return false;
		break;
	}

	UnEquipCurrenEquippable();
	CurrentEquippableInterface = NewEquippableInterface;

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("current equippable = %p"), CurrentEquippableInterface.GetInterface()));

	OnCurrentEquippableUpdate.Broadcast(NewEquippableInterface);

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("current equippable after broadcast = %p"),
	// 	                                 CurrentEquippableInterface.GetInterface()));


	return bSwitchResult;
}

void UInventoryComponent::UnEquipCurrenEquippable()
{
	AActor* CurrenEquippableActor = Cast<AActor>(CurrentEquippableInterface.GetInterface());
	if (CurrenEquippableActor)
	{
		CurrenEquippableActor->Destroy();
		CurrentEquippableInterface.SetObject(nullptr);
		CurrentEquippableInterface.SetInterface(nullptr);
	}
}


bool UInventoryComponent::HasMagnetEquipped() const
{
	return Cast<AMagnet>(GetCurrentEquippable()) != nullptr;
}

void UInventoryComponent::UnEquipCurrenEquippableWithBroadcast()
{
	UnEquipCurrenEquippable();
	OnCurrentEquippableUpdate.Broadcast(CurrentEquippableInterface);
}
