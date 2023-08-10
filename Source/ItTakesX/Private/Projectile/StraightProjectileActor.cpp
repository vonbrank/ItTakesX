// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/StraightProjectileActor.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AStraightProjectileActor::AStraightProjectileActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AStraightProjectileActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStraightProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
