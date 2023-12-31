// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/ItTakesXGameMode.h"

#include "Character/ItTakesXCharacter.h"
#include "Controller/ItTakesXController.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Environment/CheckPoint.h"
#include "Kismet/GameplayStatics.h"

void AItTakesXGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GameTimeInSeconds = GetWorld()->GetTimeSeconds();
}

void AItTakesXGameMode::BeginPlay()
{
	Super::BeginPlay();
	ItTakesXController = Cast<AItTakesXController>(GetWorld()->GetFirstPlayerController());
}

void AItTakesXGameMode::EnemyDied(AEnemyBasePawn* EnemyBasePawn)
{
	if (EnemyBasePawn)
	{
		CurrentKilledEnemyNumber += 1;
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(
		// 	                                 TEXT("CurrentKilledEnemyNumber = %d"), CurrentKilledEnemyNumber));
	}
}

void AItTakesXGameMode::CircleCrossed(AFlyingCircle* FlyingCircle)
{
	if (FlyingCircle)
	{
		CurrentCrossedCircleNumber += 1;
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(
		// 	                                 TEXT("CurrentCrossedCircleNumber = %d"), CurrentCrossedCircleNumber));
	}
}

void AItTakesXGameMode::NewUseVehicleComponents(TArray<TScriptInterface<IVehicleNode>> NewVehicleNodes)
{
	auto NewUsedVehicleComponents = CurrentUsedVehicleComponents;
	for (auto NewVehicleNode : NewVehicleNodes)
	{
		if (NewVehicleNode.GetInterface())
		{
			bool bExist = false;
			for (auto CurrentVehicleNode : CurrentUsedVehicleComponents)
			{
				if (CurrentVehicleNode.GetInterface() == NewVehicleNode.GetInterface())
				{
					bExist = true;
					break;
				}
			}
			if (!bExist)
			{
				NewUsedVehicleComponents.Add(NewVehicleNode);
			}
		}
	}
	CurrentUsedVehicleComponents = NewUsedVehicleComponents;
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("CurrentUsedVehicleComponentNumber = %d"),
	// 	                                 CurrentUsedVehicleComponents.Num()));
}

void AItTakesXGameMode::SetCharacterBeginTransform(AItTakesXCharacter* BeginCharacter, FTransform Transform)
{
	if (BeginCharacter)
	{
		Character = BeginCharacter;
		CharacterBeginTransform = Transform;
	}
}

void AItTakesXGameMode::ArriveCheckPoint(ACheckPoint* NewCheckPoint)
{
	if (NewCheckPoint)
	{
		LastCheckPoint = NewCheckPoint;
	}
}

void AItTakesXGameMode::CharacterDied(AItTakesXCharacter* DeadCharacter)
{
	if (ItTakesXController && DeadCharacter)
	{
		CurrentDiedTimes += 1;

		FTransform RespawnTransform = DeadCharacter->GetTransform();
		if (LastCheckPoint)
		{
			RespawnTransform = LastCheckPoint->GetCharacterRespawnPointTransform();
		}
		else if (Character)
		{
			RespawnTransform = CharacterBeginTransform;
		}

		ItTakesXController->CharacterDied(RespawnTransform);
	}
}

void AItTakesXGameMode::OnWinGameCheckPointRise(ADestinationCheckPoint* DestinationCheckPoint)
{
	if (ItTakesXController)
	{
		ItTakesXController->WinGame(this);
	}
}
