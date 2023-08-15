// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/VehicleComponentFlameThrower.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Enemy/EnemyBasePawn.h"
#include "Kismet/GameplayStatics.h"

AVehicleComponentFlameThrower::AVehicleComponentFlameThrower()
{
	FireParticleSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("FireParticleSpawnPoint"));
	FireParticleSpawnPoint->SetupAttachment(RootComponent);

	FireArea = CreateDefaultSubobject<UBoxComponent>(TEXT("FireArea"));
	FireArea->SetupAttachment(RootComponent);
}

void AVehicleComponentFlameThrower::BeginPlay()
{
	Super::BeginPlay();

	FireArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnFireAreaStartOverlap);
	FireArea->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnFireAreaEndOverlap);
}

void AVehicleComponentFlameThrower::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (CurrentFireParticle)
	{
		for (auto Enemy : CurrentOverlappingEnemy)
		{
			auto DamageType = UDamageType::StaticClass();
			UGameplayStatics::ApplyDamage(Enemy, DamagePerSecond * DeltaSeconds, GetWorld()->GetFirstPlayerController(),
			                              this, DamageType);
		}
	}
}


void AVehicleComponentFlameThrower::ToggleOpenFire()
{
	if (CurrentFireParticle)
	{
		for (auto Enemy : CurrentOverlappingEnemy)
		{
			Enemy->FallDownBurningIfLiving();
		}
		CurrentFireParticle->Destroy();
		CurrentFireParticle = nullptr;
	}
	else
	{
		CurrentFireParticle = GetWorld()->SpawnActor<AEmitter>(FireParticleClass,
		                                                       FireParticleSpawnPoint->GetComponentLocation(),
		                                                       FireParticleSpawnPoint->GetComponentRotation());
		CurrentFireParticle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AVehicleComponentFlameThrower::SetIsRunning(bool bNewIsRunning)
{
	Super::SetIsRunning(bNewIsRunning);

	if (!bNewIsRunning)
	{
		if (CurrentFireParticle)
		{
			CurrentFireParticle->Destroy();
			CurrentFireParticle = nullptr;
		}
	}
}

void AVehicleComponentFlameThrower::OnFireAreaStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                           bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp != OtherActor->GetRootComponent())
	{
		return;
	}

	auto NewEnemy = Cast<AEnemyBasePawn>(OtherActor);

	if (NewEnemy)
	{
		bool bIsInList = false;
		for (auto Enemy : CurrentOverlappingEnemy)
		{
			if (Enemy == NewEnemy)
			{
				bIsInList = true;
				break;
			}
		}

		if (!bIsInList)
		{
			CurrentOverlappingEnemy.Add(NewEnemy);
		}
	}
}


void AVehicleComponentFlameThrower::OnFireAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp != OtherActor->GetRootComponent())
	{
		return;
	}

	int Index = -1;
	for (int i = 0; i < CurrentOverlappingEnemy.Num(); i++)
	{
		if (CurrentOverlappingEnemy[i] == OtherActor)
		{
			Index = i;
			break;
		}
	}
	if (Index != -1)
	{
		CurrentOverlappingEnemy.RemoveAt(Index);
	}
}
