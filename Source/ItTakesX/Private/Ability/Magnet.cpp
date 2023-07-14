// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/Magnet.h"

#include "Character/ItTakesXCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AMagnet::AMagnet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("MagnetMesh");
	SetRootComponent(Mesh);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

// Called when the game starts or when spawned
void AMagnet::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::AMagnet::OnSphereStartOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::AMagnet::OnSphereEndOverlap);
}

// Called every frame
void AMagnet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMagnet::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(TEXT("On overlapping Magent: %s"), *OtherActor->GetName()));
	auto Character = Cast<AItTakesXCharacter>(OtherActor);
	if (Character)
	{
		Character->EquipMagnet(this);
	}
}

void AMagnet::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}