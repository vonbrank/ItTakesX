// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/BaseExplosion.h"
#include "GameFramework/Actor.h"
#include "Particles/Emitter.h"
#include "BaseProjectile.generated.h"

UCLASS()
class ITTAKESX_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseProjectile();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                   FVector NormalImpulse,
	                   const FHitResult& HitResult);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABaseExplosion> ExplosionClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEmitter> FireClass;


public:
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() { return ProjectileMovement; }
	FORCEINLINE TSubclassOf<AEmitter> GetFireClass() { return FireClass; }
};
