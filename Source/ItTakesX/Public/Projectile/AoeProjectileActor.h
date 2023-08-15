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

protected:
	virtual void DamageTarget(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                          FVector NormalImpulse, const FHitResult& HitResult) override;
};
