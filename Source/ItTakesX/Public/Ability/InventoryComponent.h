// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Glider.h"
#include "Magnet.h"
#include "Components/ActorComponent.h"
#include "Interface/Equippable.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FCurrentHoistableUpdateDelegate, TScriptInterface<IEquippable>, NewEquippable);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTAKESX_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// UPROPERTY()
	// TArray<TScriptInterface<IEquippable>> Equippables;

	UPROPERTY()
	TScriptInterface<IEquippable> CurrentEquippableInterface;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AMagnet> MagnetClass;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGlider> GliderClass;

	void UnEquipCurrenEquippable();
	
	bool bHaveUnArm;
	bool bHaveMagnet;
	bool bHaveGlider;

public:
	FORCEINLINE IEquippable* GetCurrentEquippable() const { return CurrentEquippableInterface.GetInterface(); }
	// FORCEINLINE void SetCurrentEquippable(TScriptInterface<IEquippable> NewEquippable)
	// {
	// 	CurrentEquippableInterface = NewEquippable;
	// 	OnCurrentEquippableUpdate.Broadcast(NewEquippable);
	// }

	void AddAndEquip(TScriptInterface<IEquippable> NewEquippableInterface);

	FCurrentHoistableUpdateDelegate OnCurrentEquippableUpdate;

	/**
	 * 
	 * @param Index 1: UnArm, 2: Magnet, 3: Glider, Others: Default
	 */
	bool SwitchToEquippableByIndex(int Index);

	void UnEquipCurrenEquippableWithBroadcast();

	bool HasMagnetEquipped() const;
};
