// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Glider.h"

// Sets default values
AGlider::AGlider()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void AGlider::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGlider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FQuat DeltaRotation = FQuat(FVector::UpVector,
	                            FMath::DegreesToRadians(RotatingSpeed) * GetWorld()->DeltaTimeSeconds);
	FQuat CurrentRotation = GetActorRotation().Quaternion();
	FQuat NewRotation = DeltaRotation * CurrentRotation;
	SetActorRotation(NewRotation);
}

EItTakesXViewType AGlider::GetViewType()
{
	return EItTakesXViewType_Normal;
}

float AGlider::GetMaxWalkingSpeed()
{
	return 600.f;
}
