// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/LightningPawn.h"

#include "BuildingSystem/VehicleControllerActor.h"
#include "Character/ItTakesXCharacter.h"
#include "GeometryCollection/GeometryCollectionComponent.h"

// Sets default values
ALightningPawn::ALightningPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightningSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("LightningSpawnPoint"));
	LightningSpawnLocation->SetupAttachment(RootComponent);

	BodyGeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("BodyGeometryCollection"));
	BodyGeometryCollection->SetupAttachment(RootComponent);

	FieldSystemSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("FieldSystemSpawnPoint"));
	FieldSystemSpawnPoint->SetupAttachment(RootComponent);

	BodyGeometryCollection->SetSimulatePhysics(false);
	BodyGeometryCollection->SetHiddenInGame(true);
	BodyGeometryCollection->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void ALightningPawn::BeginPlay()
{
	Super::BeginPlay();

	// Destruct();
}

// Called every frame
void ALightningPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTargetInRange())
	{
		if (TimeToNextShoot > 0)
		{
			TimeToNextShoot -= DeltaTime;
		}
		else
		{
			Shoot();
			TimeToNextShoot = ShootInterval;
		}

		if (CurrentLaser)
		{
			CurrentLaser->SetEndLocation(CurrentTargetCharacter->GetActorLocation());
		}
	}
	else
	{
		TimeToNextShoot = ShootInterval;
	}

	if (TimeToDestroyLaser > 0)
	{
		TimeToDestroyLaser -= DeltaTime;
	}
	else
	{
		if (CurrentLaser)
		{
			CurrentLaser->Destroy();
		}
	}
}

// Called to bind functionality to input
void ALightningPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALightningPawn::Shoot()
{
	if (CurrentLaser)
	{
		CurrentLaser->Destroy();
	}

	if (CurrentTargetCharacter == nullptr)
	{
		return;
	}

	CurrentLaser = GetWorld()->SpawnActor<ADottedLazer>(LaserClass, LightningSpawnLocation->GetComponentLocation(),
	                                                    FRotator::ZeroRotator);

	if (CurrentLaser)
	{
		TimeToDestroyLaser = ShootTime;
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("shoot laser")));

		if (CurrentTargetActor)
		{
			CurrentLaser->SetEndLocation(CurrentTargetActor->GetActorLocation());
		}
		else
		{
			auto CurrentVehicleController = CurrentTargetCharacter->GetVehicleControllerFromCharacter();
			if (CurrentVehicleController)
			{
				CurrentTargetActor = Cast<AActor>(
					CurrentVehicleController->GetRandomComponentFromVehicle().GetInterface());
				if (CurrentTargetActor)
				{
					CurrentLaser->SetEndLocation(CurrentTargetActor->GetActorLocation());
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
					                                 FString::Printf(
						                                 TEXT("New CurrentTargetActor: %s"),
						                                 *CurrentTargetActor->GetName()));
					auto VehicleNode = Cast<IVehicleNode>(CurrentTargetActor);
					if (VehicleNode)
					{
						VehicleNode->DetachFromAllAdjacentVehicleNode();
						CurrentVehicleController->RebootVehicle();
					}
				}
				else
				{
					CurrentLaser->SetEndLocation(CurrentTargetCharacter->GetActorLocation());
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
					                                 FString::Printf(
						                                 TEXT("CurrentTargetActor is null")));
				}
				CurrentTargetActor = nullptr;
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
				                                 FString::Printf(
					                                 TEXT("CurrentVehicleController is null")));
				CurrentLaser->SetEndLocation(CurrentTargetCharacter->GetActorLocation());
			}
		}
	}
}

void ALightningPawn::Destruct(AActor* DestructCauser, AController* DestructInstigator)
{
	Super::Destruct(DestructCauser, DestructInstigator);

	BodyMesh->SetSimulatePhysics(false);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BodyMesh->SetHiddenInGame(true);

	BodyGeometryCollection->SetHiddenInGame(false);
	BodyGeometryCollection->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BodyGeometryCollection->SetSimulatePhysics(true);

	GetWorld()->SpawnActor<AFieldSystemActor>(FieldSystemClass, FieldSystemSpawnPoint->GetComponentLocation(),
	                                          FieldSystemSpawnPoint->GetComponentRotation());
}

void ALightningPawn::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	Super::OnSphereStartOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	//
	// AItTakesXCharacter* NewCharacter = Cast<AItTakesXCharacter>(OtherActor);
	//
	// if (NewCharacter)
	// {
	// }
}

void ALightningPawn::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor == CurrentTargetCharacter)
	{
		CurrentTargetCharacter = nullptr;
		CurrentTargetActor = nullptr;
	}
}
