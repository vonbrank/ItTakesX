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
	UGameplayStatics::ApplyDamage(OtherActor, HitDamage, ThisInstigator, this, DamageType);
	UGameplayStatics::ApplyRadialDamage(this, RadialDamage, HitResult.Location, DamageRadial, DamageType, IgnoreActors,
	                                    this,
	                                    ThisInstigator);

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("Projectile on hit, OtherActor: %s"), *OtherActor->GetName()));
}
