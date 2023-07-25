// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/BombActor.h"

#include "Environment/TimeReversingActor.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ABombActor::ABombActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BombMesh");
	SetRootComponent(Mesh);

	// RadialForceComponent = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
}

// Called when the game starts or when spawned
void ABombActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABombActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABombActor::Explode()
{
	if (bHasExplode)
	{
		HandHasExploded();
	}
	else
	{
		if (ExplosionActorClass)
		{
			TArray<AActor*> OutActors;
			UGameplayStatics::GetAllActorsOfClass(this, ATimeReversingActor::StaticClass(), OutActors);
			for (auto Actor : OutActors)
			{
				ATimeReversingActor* TimeReversingActor = Cast<ATimeReversingActor>(Actor);
				if (TimeReversingActor == nullptr)
				{
					continue;
				}
				if ((TimeReversingActor->GetActorLocation() - GetActorLocation()).Length() > 500.f)
				{
					continue;
				}

				AreaTimeReversingActors.Add(TimeReversingActor);
			}

			for (auto Actor : AreaTimeReversingActors)
			{
				Actor->StartRecording(CaptureTime);
			}

			auto ExplosionActor = GetWorld()->SpawnActor<AActor>(ExplosionActorClass);
			if (ExplosionActor)
			{
				ExplosionActor->SetActorLocation(GetActorLocation());
				bHasExplode = true;
			}
		}
	}
}

void ABombActor::TimeReverse()
{
	for (auto Actor : AreaTimeReversingActors)
	{
		Actor->StartReplaying();
	}
}

void ABombActor::ExplodeAgain()
{
	for (auto Actor : AreaTimeReversingActors)
	{
		Actor->StartRecording(CaptureTime);
	}

	auto ExplosionActor = GetWorld()->SpawnActor<AActor>(ExplosionActorClass);
	if (ExplosionActor)
	{
		ExplosionActor->SetActorLocation(GetActorLocation());
	}
}
