// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/WindField.h"

#include "Character/ItTakesXCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AWindField::AWindField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WindBody = CreateDefaultSubobject<UStaticMeshComponent>("WindBody");
	SetRootComponent(WindBody);
}

// Called when the game starts or when spawned
void AWindField::BeginPlay()
{
	Super::BeginPlay();
	WindBody->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AWindField::OnBodyStartOverlap);
	WindBody->OnComponentEndOverlap.AddDynamic(this, &ThisClass::AWindField::OnBodyEndOverlap);
}

// Called every frame
void AWindField::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CurrenOverlappingCharacter)
	{
		CurrenOverlappingCharacter->GetCharacterMovement()->AddForce(FVector::UpVector * ForceLength);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Adding force")));
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
