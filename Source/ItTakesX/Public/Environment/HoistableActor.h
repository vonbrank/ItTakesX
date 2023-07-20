// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Aimable.h"
#include "Interface/Hoistable.h"
#include "HoistableActor.generated.h"

UCLASS()
class ITTAKESX_API AHoistableActor : public AActor, public IAimable, public IHoistable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHoistableActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
protected:
	UPROPERTY()
	AActor* CurrentAimingActor;
	UPROPERTY()
	AActor* CurrentHoistingActor;

public:
	virtual void OnBeginAiming_Implementation(AActor* OtherActor);
	virtual void OnEndAiming_Implementation(AActor* OtherActor);
	virtual void OnBeginHoisting_Implementation(AActor* OtherActor);
	virtual void OnEndHoisting_Implementation(AActor* OtherActor);
	virtual bool IsHoisting() const override;
};
