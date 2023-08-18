// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadZone.generated.h"

UCLASS()
class ITTAKESX_API ADeadZone : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADeadZone();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* AreaBox;

	UFUNCTION()
	virtual void OnBoxStartOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	virtual void OnBoxEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UPROPERTY()
	class AItTakesXCharacter* CurrentCharacter;

	UPROPERTY(EditAnywhere)
	float DamageSpeed = 100;

	UPROPERTY(EditAnywhere)
	float CharacterMaxSpeed = 40000;

public:
};
