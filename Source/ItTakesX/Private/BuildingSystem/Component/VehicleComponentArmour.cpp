// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/VehicleComponentArmour.h"

#include "Components/StaticMeshComponent.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

AVehicleComponentArmour::AVehicleComponentArmour()
{
	BodyMesh75Health = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh75Health"));
	BodyMesh75Health->SetupAttachment(RootComponent);
	BodyMesh50Health = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh50Health"));
	BodyMesh50Health->SetupAttachment(RootComponent);
	BodyMesh25Health = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh25Health"));
	BodyMesh25Health->SetupAttachment(RootComponent);

	BodyMesh75Health->SetSimulatePhysics(false);
	BodyMesh75Health->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh75Health->SetHiddenInGame(true);

	BodyMesh50Health->SetSimulatePhysics(false);
	BodyMesh50Health->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh50Health->SetHiddenInGame(true);

	BodyMesh25Health->SetSimulatePhysics(false);
	BodyMesh25Health->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh25Health->SetHiddenInGame(true);
}

void AVehicleComponentArmour::Destruct75()
{
	Mesh->SetHiddenInGame(true);
	BodyMesh75Health->SetHiddenInGame(false);
	BodyMesh75Health->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetWorld()->SpawnActor<AArmourGeometryCollection>(Destruct75GeometryClass, GetActorLocation(), GetActorRotation());
}

void AVehicleComponentArmour::Destruct50()
{
	BodyMesh75Health->SetHiddenInGame(true);
	BodyMesh75Health->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh50Health->SetHiddenInGame(false);
	BodyMesh50Health->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetWorld()->SpawnActor<AArmourGeometryCollection>(Destruct50GeometryClass, GetActorLocation(), GetActorRotation());
}

void AVehicleComponentArmour::Destruct25()
{
	BodyMesh50Health->SetHiddenInGame(true);
	BodyMesh50Health->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh25Health->SetHiddenInGame(false);
	BodyMesh25Health->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetWorld()->SpawnActor<AArmourGeometryCollection>(Destruct25GeometryClass, GetActorLocation(), GetActorRotation());
}

void AVehicleComponentArmour::Destruct00()
{
	BodyMesh25Health->SetHiddenInGame(true);
	BodyMesh25Health->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorld()->SpawnActor<AArmourGeometryCollection>(Destruct00GeometryClass, GetActorLocation(), GetActorRotation());
	DetachFromParentVehicleNode();
}
