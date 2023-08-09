// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/StraightProjectileActor.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AStraightProjectileActor::AStraightProjectileActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
}

// Called when the game starts or when spawned
void AStraightProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &ThisClass::OnHit);
}

// Called every frame
void AStraightProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStraightProjectileActor::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                     FVector NormalImpulse, const FHitResult& HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("OnHit"));
	Destroy();
}
