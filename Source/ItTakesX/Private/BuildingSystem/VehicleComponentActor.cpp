// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/VehicleComponentActor.h"

#include "Components/SphereComponent.h"

// Sets default values
AVehicleComponentActor::AVehicleComponentActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("VehicleMesh");
	SetRootComponent(Mesh);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	// AreaSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void AVehicleComponentActor::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetSimulatePhysics(true);
}

// Called every frame
void AVehicleComponentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVehicleComponentActor::OnBeginAiming_Implementation(AActor* OtherActor)
{
	CurrentAimingActor = OtherActor;
}

void AVehicleComponentActor::OnEndAiming_Implementation(AActor* OtherActor)
{
	CurrentAimingActor = nullptr;
}

void AVehicleComponentActor::OnBeginHoisting_Implementation(AActor* OtherActor)
{
	CurrentHoistingActor = OtherActor;
}

void AVehicleComponentActor::OnEndHoisting_Implementation(AActor* OtherActor)
{
	CurrentHoistingActor = nullptr;
}
