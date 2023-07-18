// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/BounceActor.h"

#include "Character/ItTakesXCharacter.h"

// Sets default values
ABounceActor::ABounceActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("BounceMesh");
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ABounceActor::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &ThisClass::OnMeshHit);
}

// Called every frame
void ABounceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABounceActor::OnMeshHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             FVector NormalImpulse, const FHitResult& Hit)
{
	AItTakesXCharacter* Character = Cast<AItTakesXCharacter>(OtherActor);

	if (Character)
	{
		auto Angle = FMath::RadiansToDegrees(FMath::Acos(
			FVector::DotProduct(FVector::DownVector, Hit.Normal) / (Hit.Normal.Length() * FVector::DownVector.
				Length())));

		if (Angle < BouncingThresholdAngle)
		{
			Character->LaunchCharacter(FVector::UpVector * 1000, false, false);
		}
	}
}
