// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/VehicleComponentArmour.h"

#include "BuildingSystem/VehicleControllerActor.h"
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

void AVehicleComponentArmour::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	OnTakeAnyDamage.AddDynamic(this, &ThisClass::DamageTaken);
}

// void AVehicleComponentArmour::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
//                                           AController* DamageInstigator, AActor* DamageCauser)
// {
// 	float PreviousHealth = Health;
// 	Health -= Damage;
//
// 	if (PreviousHealth / MaxHealth >= 0.75 && Health / MaxHealth < 0.75)
// 	{
// 		Destruct75();
// 	}
// 	if (PreviousHealth / MaxHealth >= 0.50 && Health / MaxHealth < 0.50)
// 	{
// 		Destruct50();
// 	}
// 	if (PreviousHealth / MaxHealth >= 0.25 && Health / MaxHealth < 0.25)
// 	{
// 		Destruct25();
// 	}
// 	if (PreviousHealth / MaxHealth >= 0 && Health / MaxHealth < 0)
// 	{
// 		Destruct00();
// 	}
//
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
// 	                                 FString::Printf(
// 		                                 TEXT("AVehicleComponentArmour before = %f, after = %f, current Health = %f"),
// 		                                 PreviousHealth / MaxHealth, Health / MaxHealth, Health));
// }

void AVehicleComponentArmour::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                          AController* DamageInstigator, AActor* DamageCauser)
{
	// 装甲不需要将伤害转发到整个载具
	// Super::DamageTaken(DamagedActor, Damage, DamageType, DamageInstigator, DamageCauser);

	float PreviousHealth = Health;
	Health -= Damage;

	if (PreviousHealth / MaxHealth >= 0.75 && Health / MaxHealth < 0.75)
	{
		Destruct75();
	}
	if (PreviousHealth / MaxHealth >= 0.50 && Health / MaxHealth < 0.50)
	{
		Destruct50();
	}
	if (PreviousHealth / MaxHealth >= 0.25 && Health / MaxHealth < 0.25)
	{
		Destruct25();
	}
	if (PreviousHealth / MaxHealth >= 0 && Health / MaxHealth < 0)
	{
		Destruct00();
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(
		                                 TEXT("AVehicleComponentArmour before = %f, after = %f, current Health = %f"),
		                                 PreviousHealth / MaxHealth, Health / MaxHealth, Health));

	auto VehicleController = Cast<AVehicleControllerActor>(CurrentRunningVehicleController.GetInterface());
	if (VehicleController)
	{
		VehicleController->UpdateArmourHealth();
	}
}

void AVehicleComponentArmour::Destruct75()
{
	Mesh->SetHiddenInGame(true);
	BodyMesh75Health->SetHiddenInGame(false);
	BodyMesh75Health->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	if (CurrentArmourGeometryCollection)
	{
		CurrentArmourGeometryCollection->Destroy();
	}
	CurrentArmourGeometryCollection = GetWorld()->SpawnActor<AArmourGeometryCollection>(
		Destruct75GeometryClass, GetActorLocation(), GetActorRotation());
	if (CurrentArmourGeometryCollection)
	{
		CurrentArmourGeometryCollection->SetLifeSpan(ArmourGeometryCollectionLifeSpan);
	}
}

void AVehicleComponentArmour::Destruct50()
{
	BodyMesh75Health->SetHiddenInGame(true);
	BodyMesh75Health->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh50Health->SetHiddenInGame(false);
	BodyMesh50Health->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (CurrentArmourGeometryCollection)
	{
		CurrentArmourGeometryCollection->Destroy();
	}
	CurrentArmourGeometryCollection = GetWorld()->SpawnActor<AArmourGeometryCollection>(
		Destruct50GeometryClass, GetActorLocation(), GetActorRotation());
	if (CurrentArmourGeometryCollection)
	{
		CurrentArmourGeometryCollection->SetLifeSpan(ArmourGeometryCollectionLifeSpan);
	}
}

void AVehicleComponentArmour::Destruct25()
{
	BodyMesh50Health->SetHiddenInGame(true);
	BodyMesh50Health->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh25Health->SetHiddenInGame(false);
	BodyMesh25Health->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	if (CurrentArmourGeometryCollection)
	{
		CurrentArmourGeometryCollection->Destroy();
	}
	CurrentArmourGeometryCollection = GetWorld()->SpawnActor<AArmourGeometryCollection>(
		Destruct25GeometryClass, GetActorLocation(), GetActorRotation());
	if (CurrentArmourGeometryCollection)
	{
		CurrentArmourGeometryCollection->SetLifeSpan(ArmourGeometryCollectionLifeSpan);
	}
}

void AVehicleComponentArmour::Destruct00()
{
	BodyMesh25Health->SetHiddenInGame(true);
	BodyMesh25Health->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (CurrentArmourGeometryCollection)
	{
		CurrentArmourGeometryCollection->Destroy();
	}
	CurrentArmourGeometryCollection = GetWorld()->SpawnActor<AArmourGeometryCollection>(
		Destruct00GeometryClass, GetActorLocation(), GetActorRotation());
	if (CurrentArmourGeometryCollection)
	{
		CurrentArmourGeometryCollection->SetLifeSpan(ArmourGeometryCollectionLifeSpan);
	}
	DetachFromAllAdjacentVehicleNode();

	auto VehicleController = Cast<AVehicleControllerActor>(CurrentRunningVehicleController.GetInterface());
	if (VehicleController)
	{
		VehicleController->RebootVehicle();
	}

	// Destroy();
}
