// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "GameFramework/Actor.h"
#include "ChaosForceField.generated.h"

UCLASS()
class ITTAKESX_API AChaosForceField : public AFieldSystemActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AChaosForceField();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	float RadialFallOffFieldMagnitude;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = true))
	float RadialVectorFieldMagnitude;

public:
	FORCEINLINE void SetRadialFallOffFieldMagnitude(float Value) { RadialFallOffFieldMagnitude = Value; }
	FORCEINLINE void SetRadialVectorFieldMagnitude(float Value) { RadialVectorFieldMagnitude = Value; }
};
