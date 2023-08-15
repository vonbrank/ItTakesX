// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AoeProjectileActor.h"

#include "PhysicsEngine/RadialForceActor.h"
#include "PhysicsEngine/RadialForceComponent.h"

void AAoeProjectileActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                FVector NormalImpulse, const FHitResult& HitResult)
{
	auto Explosion = GetWorld()->SpawnActor<ABaseExplosion>(ExplosionClass, HitResult.Location, FRotator::ZeroRotator);
	if (Explosion)
	{
		Explosion->SetLifeSpan(1);
	}
	Destroy();
}
