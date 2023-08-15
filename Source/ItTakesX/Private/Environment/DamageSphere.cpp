// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/DamageSphere.h"

// Sets default values
ADamageSphere::ADamageSphere()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ADamageSphere::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADamageSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
