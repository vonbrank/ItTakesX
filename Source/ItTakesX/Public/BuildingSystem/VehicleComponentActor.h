// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Aimable.h"
#include "Interface/Hoistable.h"
#include "VehicleComponentActor.generated.h"

UCLASS()
class ITTAKESX_API AVehicleComponentActor : public AActor, public IAimable, public IHoistable
{
	GENERATED_BODY()

public:
	AVehicleComponentActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Vehicle Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Vehicle Properties")
	class UStaticMeshComponent* Mesh;

	UPROPERTY()
	AActor* CurrentAimingActor;
	UPROPERTY()
	AActor* CurrentHoistingActor;

public:
	void OnBeginAiming_Implementation(AActor* OtherActor);
	void OnEndAiming_Implementation(AActor* OtherActor);
	void OnBeginHoisting_Implementation(AActor* OtherActor);
	void OnEndHoisting_Implementation(AActor* OtherActor);
};
