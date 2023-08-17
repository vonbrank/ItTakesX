// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/CheckPoint.h"

#include "Components/ArrowComponent.h"
#include "GameModes/ItTakesXGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	ButtonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonBase"));
	ButtonBase->SetupAttachment(RootComponent);

	ButtonBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonBody"));
	ButtonBody->SetupAttachment(ButtonBase);

	ButtonConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("ButtonConstraint"));
	ButtonConstraint->SetupAttachment(ButtonBase);

	ButtonConstraint->SetConstrainedComponents(ButtonBase, NAME_None, ButtonBody, NAME_None);

	TokenCubeSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("TokenCubeSpawnPoint"));
	TokenCubeSpawnPoint->SetupAttachment(RootComponent);

	FlagBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagBase"));
	FlagBase->SetupAttachment(RootComponent);

	FlagBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagBody"));
	FlagBody->SetupAttachment(FlagBase);

	FlagConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("FlagConstraint"));
	FlagConstraint->SetupAttachment(FlagBase);

	CharacterRespawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CharacterRespawnPoint"));
	CharacterRespawnPoint->SetupAttachment(RootComponent);

	FlagConstraint->SetConstrainedComponents(FlagBase, NAME_None, FlagBody, NAME_None);
}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	ButtonBodyDynamicMaterial = ButtonBody->CreateDynamicMaterialInstance(0);
	ItTakesXGameMode = Cast<AItTakesXGameMode>(UGameplayStatics::GetGameMode(this));
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("button body height: %f"),
	// 	                                 GetTransform().InverseTransformPosition(
	// 		                                 ButtonBody->GetComponentLocation()).Z));

	if (GetTransform().InverseTransformPosition(
		ButtonBody->GetComponentLocation()).Z < ActiveThreshold)
	{
		CurrentPressingTime += DeltaTime;

		LightUpButton();

		if (CanSpawnTokenCube())
		{
			bCurrentHasActive = true;
			SpawnTokenCube();

			if (!bHasRoseFlag)
			{
				RiseFlag();
				bHasRoseFlag = true;
			}
		}
	}
	else
	{
		ResetButtonState();
	}
}

void ACheckPoint::SpawnTokenCube()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("spawn token cube")));
	CurrentSpawnActor = GetWorld()->SpawnActor<AActor>(TokenCubeClass, TokenCubeSpawnPoint->GetComponentLocation(),
	                                                   TokenCubeSpawnPoint->GetComponentRotation());
	if (CurrentSpawnActor)
	{
		CurrentSpawnActor->Tags.Add(SpawnCubeTag);
	}
	ItTakesXGameMode->ArriveCheckPoint(this);
}

void ACheckPoint::RiseFlag()
{
	FlagConstraint->SetLinearDriveParams(1000, 200, 0);
	FlagBody->AddForce(FlagBody->GetUpVector());
}

FTransform ACheckPoint::GetCharacterRespawnPointTransform()
{
	return CharacterRespawnPoint->GetComponentTransform();
}

bool ACheckPoint::CanSpawnTokenCube()
{
	return !bCurrentHasActive && CurrentPressingTime >= ActivePressingTime;
}

void ACheckPoint::ResetButtonState()
{
	CurrentPressingTime = 0;
	bCurrentHasActive = false;
	LightDownButton();
}

void ACheckPoint::LightUpButton()
{
	ButtonBodyDynamicMaterial->SetVectorParameterValue(TEXT("Color_Tint_R"), ButtonLightUpColor);
	ButtonBodyDynamicMaterial->SetVectorParameterValue(TEXT("Emissive_Color"), ButtonLightUpColor);
}

void ACheckPoint::LightDownButton()
{
	ButtonBodyDynamicMaterial->SetVectorParameterValue(TEXT("Color_Tint_R"), ButtonLightDownColor);
	ButtonBodyDynamicMaterial->SetVectorParameterValue(TEXT("Emissive_Color"), ButtonLightDownColor);
}
