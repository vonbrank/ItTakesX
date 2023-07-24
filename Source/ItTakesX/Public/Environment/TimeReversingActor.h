// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimeReversingActor.generated.h"

UCLASS()
class ITTAKESX_API ATimeReversingActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATimeReversingActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY()
	TArray<FTransform> TransformList;

	UPROPERTY()
	TArray<FTransform> VelocityList;

	bool bReversing;

	UPROPERTY(EditAnywhere)
	float RecordTime = 10.f;

	float CurrentLeftTime;

protected:
public:
	UFUNCTION(BlueprintCallable)
	void StartRecording();

	UFUNCTION(BlueprintCallable)
	void StartReplaying();
};
