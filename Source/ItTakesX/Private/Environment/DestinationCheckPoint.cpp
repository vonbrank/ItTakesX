// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/DestinationCheckPoint.h"

void ADestinationCheckPoint::RiseFlag()
{
	Super::RiseFlag();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("You win the game")));
}
