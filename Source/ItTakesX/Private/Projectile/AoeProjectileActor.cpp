// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AoeProjectileActor.h"

#include "Enemy/EnemyBasePawn.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceActor.h"
#include "PhysicsEngine/RadialForceComponent.h"

void AAoeProjectileActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                FVector NormalImpulse, const FHitResult& HitResult)
{
	auto ThisOwner = GetOwner();
	AController* ThisInstigator = nullptr;

	auto EnemyPawn = Cast<AEnemyBasePawn>(ThisOwner);
	if (EnemyPawn)
	{
		ThisInstigator = EnemyPawn->GetController();
	}
	else
	{
		ThisInstigator = UGameplayStatics::GetPlayerController(this, 0);
	}

	auto DamageType = UDamageType::StaticClass();
	TArray<AActor*> IgnoreActors;
	UGameplayStatics::ApplyRadialDamage(this, 1000, HitResult.Location, 500, DamageType, IgnoreActors, this,
	                                    ThisInstigator);

	auto Explosion = GetWorld()->SpawnActor<ABaseExplosion>(ExplosionClass, HitResult.Location, FRotator::ZeroRotator);
	if (Explosion)
	{
		Explosion->SetLifeSpan(1);
	}
	Destroy();
}
