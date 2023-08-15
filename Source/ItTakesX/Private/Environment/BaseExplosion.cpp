// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/BaseExplosion.h"

#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ABaseExplosion::ABaseExplosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));

	ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	ParticleSystem->SetupAttachment(RootComponent);

	RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseExplosion::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABaseExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
