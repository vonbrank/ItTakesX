// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	UPROPERTY()
	TArray<TScriptInterface<IEquippable>> Equippables;

	UPROPERTY()
	TScriptInterface<IEquippable> CurrentEquippable;


public:
	FORCEINLINE IEquippable* GetCurrentEquippable() const { return CurrentEquippable.GetInterface(); }
	FORCEINLINE void SetCurrentEquippable(TScriptInterface<IEquippable> NewEquippable)
	{
		CurrentEquippable = NewEquippable;
		OnCurrentEquippableUpdate.Broadcast(NewEquippable);
	}

	void AddAndEquip(TScriptInterface<IEquippable> NewEquippable);

	FCurrentHoistableUpdateDelegate OnCurrentEquippableUpdate;

	bool HasMagnetEquipped() const;
};
