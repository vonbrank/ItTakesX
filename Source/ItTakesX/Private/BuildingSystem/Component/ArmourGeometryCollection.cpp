// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/ArmourGeometryCollection.h"

#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
AArmourGeometryCollection::AArmourGeometryCollection()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyGeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("BodyGeometryCollection"));
	SetRootComponent(BodyGeometryCollection);
}

// Called when the game starts or when spawned
void AArmourGeometryCollection::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->SpawnActor<AFieldSystemActor>(FieldSystemClass, GetActorLocation(), GetActorRotation());
}

// Called every frame
void AArmourGeometryCollection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
