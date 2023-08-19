// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/DestinationCheckPoint.h"

#include "GameModes/ItTakesXGameMode.h"
#include "Kismet/GameplayStatics.h"

void ADestinationCheckPoint::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AItTakesXGameMode>(UGameplayStatics::GetGameMode(this));
}

void ADestinationCheckPoint::RiseFlag()
{
	Super::RiseFlag();

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("You win the game")));
	if (GameMode)
	{
		GameMode->OnWinGameCheckPointRise(this);
	}
}
