// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/PickupableActor.h"
#include "Interface/Equippable.h"
#include "Magnet.generated.h"

UCLASS()
class ITTAKESX_API AMagnet : public APickupableActor, public IEquippable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMagnet();
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* LaserEffectSpawnPoint;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADottedLazer> MagnetEffectClass;

	UPROPERTY()
	class ADottedLazer* CurrentMagnetEffect;

protected:
	virtual void OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                  const FHitResult& SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

public:
	FORCEINLINE virtual EItTakesXViewType GetViewType() override { return EItTakesXViewType_NiceToAiming; };
	FORCEINLINE virtual float GetMaxWalkingSpeed() override { return 300.f; };

	class ADottedLazer* GetCurrentMagnetEffect();
	class ADottedLazer* SpawnNewMagnetEffect();
	void DestroyCurrentMagnetEffect();
};
