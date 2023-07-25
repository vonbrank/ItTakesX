// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HoistableActor.h"
#include "TimeReversingActor.h"
#include "GameFramework/Actor.h"
#include "Interface/Aimable.h"
#include "BombActor.generated.h"

UCLASS()
class ITTAKESX_API ABombActor : public AHoistableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABombActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ExplosionActorClass;


	bool bHasExplode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float CaptureTime = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float ExplodeAgainInterval = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsReversing;

	UPROPERTY()
	TArray<ATimeReversingActor*> AreaTimeReversingActors;


public:
	void Explode();

	UFUNCTION(BlueprintCallable)
	void TimeReverse();
	UFUNCTION(BlueprintCallable)
	void ExplodeAgain();

	UFUNCTION(BlueprintImplementableEvent)
	void HandHasExploded();
};
