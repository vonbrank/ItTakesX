// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseExplosion.generated.h"

UCLASS()
class ITTAKESX_API ABaseExplosion : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseExplosion();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	class UParticleSystemComponent* ParticleSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class URadialForceComponent* RadialForce;
public:
	FORCEINLINE URadialForceComponent* GetRadialForce() { return RadialForce; }
};
