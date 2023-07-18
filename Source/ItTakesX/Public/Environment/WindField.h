// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindField.generated.h"

UCLASS()
class ITTAKESX_API AWindField : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWindField();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* WindBody;

	UFUNCTION()
	virtual void OnBodyStartOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	virtual void OnBodyEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float ForceLength;

	UPROPERTY()
	class AItTakesXCharacter* CurrenOverlappingCharacter;

protected:
public:
};
