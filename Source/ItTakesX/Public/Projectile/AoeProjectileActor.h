// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/BaseProjectile.h"
#include "AoeProjectileActor.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AAoeProjectileActor : public ABaseProjectile
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	float HitDamage = 10;

	UPROPERTY(EditAnywhere)
	float RadialDamage = 40;

	UPROPERTY(EditAnywhere)
	float DamageRadial = 800;
protected:
	virtual void DamageTarget(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          FVector NormalImpulse, const FHitResult& HitResult) override;
};
