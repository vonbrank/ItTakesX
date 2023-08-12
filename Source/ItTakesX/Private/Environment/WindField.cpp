// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/WindField.h"

#include "NiagaraComponent.h"
#include "Character/ItTakesXCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AWindField::AWindField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ForceDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("ForceDirection"));
	SetRootComponent(ForceDirection);

	WindArea = CreateDefaultSubobject<UBoxComponent>("WindArea");
	WindArea->SetupAttachment(RootComponent);

	ForceParticle = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ForceParticle"));
	ForceParticle->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWindField::BeginPlay()
{
	Super::BeginPlay();
	WindArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AWindField::OnBodyStartOverlap);
	WindArea->OnComponentEndOverlap.AddDynamic(this, &ThisClass::AWindField::OnBodyEndOverlap);
}

// Called every frame
void AWindField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrenOverlappingCharacter && CurrenOverlappingCharacter->IsGliding())
	{
		auto CurrentVelocity = CurrenOverlappingCharacter->GetCharacterMovement()->Velocity;

		CurrenOverlappingCharacter->GetCharacterMovement()->
		                            AddForce(-CurrentVelocity * AirDrag);

		CurrenOverlappingCharacter->GetCharacterMovement()->
		                            AddForce(
			                            ForceDirection->GetForwardVector() * ForceLength * CurrenOverlappingCharacter->
			                            GetCharacterMovement()->Mass);

		float CurrentForwardSpeedLength = FVector::DotProduct(ForceDirection->GetForwardVector(),
		                                                      CurrenOverlappingCharacter->GetCharacterMovement()->
		                                                      Velocity);

		if (CurrentForwardSpeedLength > MaxSpeed)
		{
			CurrenOverlappingCharacter->GetCharacterMovement()->Velocity = (
				CurrenOverlappingCharacter->GetCharacterMovement()->Velocity / CurrentForwardSpeedLength * MaxSpeed);
		}


		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Adding force")));
	}
}

void AWindField::OnBodyStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	auto Character = Cast<AItTakesXCharacter>(OtherActor);

	if (Character)
	{
		CurrenOverlappingCharacter = Character;
	}
}

void AWindField::OnBodyEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (CurrenOverlappingCharacter == OtherActor)
	{
		CurrenOverlappingCharacter = nullptr;
	}
}
