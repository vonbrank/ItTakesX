// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/TimeReversingActor.h"

// Sets default values
ATimeReversingActor::ATimeReversingActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ATimeReversingActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATimeReversingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bReversing)
	{
		int LastIndex = TransformList.Num() - 1;

		if (TransformList.IsValidIndex(LastIndex))
		{
			FTransform Transform = TransformList[LastIndex];
			FVector Velocity = VelocityList[LastIndex].GetLocation();
			FVector Angular = VelocityList[LastIndex].GetScale3D();
			SetActorTransform(Transform);
			Mesh->SetPhysicsLinearVelocity(Velocity);
			Mesh->SetPhysicsAngularVelocityInRadians(Angular);
			TransformList.Pop();
		}
	}
	else if (CurrentLeftTime >= 0)
	{
		FTransform Transform = GetActorTransform();
		FVector Velocity = Mesh->GetPhysicsLinearVelocity();
		FVector Angular = Mesh->GetPhysicsAngularVelocityInRadians();

		FTransform NewVelocity;
		NewVelocity.SetLocation(Velocity);
		NewVelocity.SetScale3D(Angular);
		TransformList.Add(Transform);
		VelocityList.Add(NewVelocity);

		CurrentLeftTime -= GetWorld()->DeltaTimeSeconds;
	}
}

void ATimeReversingActor::StartRecording(float RecordTime)
{
	bReversing = false;
	CurrentLeftTime = RecordTime;
}

void ATimeReversingActor::StartReplaying()
{
	bReversing = true;
	CurrentLeftTime = 0;
}
