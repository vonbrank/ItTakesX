// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/AoeProjectileActor.h"

#include "Enemy/EnemyBasePawn.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceActor.h"
#include "PhysicsEngine/RadialForceComponent.h"


void AAoeProjectileActor::DamageTarget(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
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
		ThisInstigator = GetWorld()->GetFirstPlayerController();
	}

	auto DamageType = UDamageType::StaticClass();
	TArray<AActor*> IgnoreActors;
	UGameplayStatics::ApplyDamage(OtherActor, 10, ThisInstigator, this, DamageType);
	UGameplayStatics::ApplyRadialDamage(this, 20, HitResult.Location, 500, DamageType, IgnoreActors, this,
	                                    ThisInstigator);

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("Projectile on hit, OtherActor: %s"), *OtherActor->GetName()));
}
