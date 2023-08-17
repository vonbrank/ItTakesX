// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/FlyingCircle.h"

#include "Character/ItTakesXCharacter.h"
#include "Components/BoxComponent.h"
#include "GameModes/ItTakesXGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFlyingCircle::AFlyingCircle()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AreaBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AreaBox"));
	SetRootComponent(AreaBox);

	FirstPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirstPlane"));
	FirstPlane->SetupAttachment(RootComponent);

	SecondPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondPlane"));
	SecondPlane->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AFlyingCircle::BeginPlay()
{
	Super::BeginPlay();

	AreaBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBoxStartOverlap);
	AreaBox->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnBoxEndOverlap);

	FirstPlaneMaterial = FirstPlane->CreateDynamicMaterialInstance(0);
	SecondPlaneMaterial = SecondPlane->CreateDynamicMaterialInstance(0);
	if (FirstPlaneMaterial)
	{
		// FirstPlaneMaterial->SetScalarParameterValue(TEXT("ColorBright"), 10);
		FirstPlaneMaterial->SetVectorParameterValue(FName("BaseColor"), OriginColor);
	}
	if (SecondPlaneMaterial)
	{
		// SecondPlaneMaterial->SetScalarParameterValue(TEXT("ColorBright"), 10);
		SecondPlaneMaterial->SetVectorParameterValue(FName("BaseColor"), OriginColor);
	}

	ItTakesXGameMode = Cast<AItTakesXGameMode>(UGameplayStatics::GetGameMode(this));
}

// Called every frame
void AFlyingCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFlyingCircle::OnBoxStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	auto Character = Cast<AItTakesXCharacter>(OtherActor);
	if (Character && bCharacterHasCrossed == false)
	{
		bCharacterHasCrossed = true;
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Character first Overlap")));

		if (FirstPlaneMaterial)
		{
			// FirstPlaneMaterial->SetScalarParameterValue(TEXT("ColorBright"), 10);
			FirstPlaneMaterial->SetVectorParameterValue(FName("BaseColor"), CrossColor);
		}
		if (SecondPlaneMaterial)
		{
			// SecondPlaneMaterial->SetScalarParameterValue(TEXT("ColorBright"), 10);
			SecondPlaneMaterial->SetVectorParameterValue(FName("BaseColor"), CrossColor);
		}

		if (ItTakesXGameMode)
		{
			ItTakesXGameMode->CircleCrossed(this);
		}
	}
}

void AFlyingCircle::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
