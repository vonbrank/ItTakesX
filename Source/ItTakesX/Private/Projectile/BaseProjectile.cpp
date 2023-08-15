// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/BaseProjectile.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceActor.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& HitResult)
{
	Destroy();
}
