// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DottedLazer.generated.h"

UCLASS()
class ITTAKESX_API ADottedLazer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADottedLazer();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UNiagaraComponent* NiagaraComp;

public:
	void SetEndLocation(FVector Location);
};
