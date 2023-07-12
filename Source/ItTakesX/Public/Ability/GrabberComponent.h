// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/Hoistable.h"
#include "GrabberComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTAKESX_API UGrabberComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGrabberComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class UAimingComponent* AimingComponent;

	IHoistable* CurrentHoistingHoistable;

	float CurrentSelectDistance;

	bool InteractWithHoisting();


public:
	bool ToggleHoistingActor();
	bool InterfaceWithComposing();
};
