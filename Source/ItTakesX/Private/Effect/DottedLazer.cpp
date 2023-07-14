// Fill out your copyright notice in the Description page of Project Settings.


#include "Effect/DottedLazer.h"
#include "NiagaraComponent.h"

// Sets default values
ADottedLazer::ADottedLazer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>("Niagara");
}

// Called when the game starts or when spawned
void ADottedLazer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADottedLazer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADottedLazer::SetEndLocation(FVector Location)
{
	NiagaraComp->SetVectorParameter(TEXT("LaserEnd"), Location);
}
