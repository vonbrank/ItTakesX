// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/LightningPawn.h"

#include "Character/ItTakesXCharacter.h"

// Sets default values
ALightningPawn::ALightningPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightningSpawnLocation = CreateDefaultSubobject<USceneComponent>(TEXT("LightningSpawnPoint"));
	LightningSpawnLocation->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALightningPawn::BeginPlay()
{
	Super::BeginPlay();
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
		CurrentLaser->SetEndLocation(CurrentTargetCharacter->GetActorLocation());
	}
}
