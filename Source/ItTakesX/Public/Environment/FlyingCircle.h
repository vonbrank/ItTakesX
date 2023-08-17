// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FlyingCircle.generated.h"

UCLASS()
class ITTAKESX_API AFlyingCircle : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFlyingCircle();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* FirstPlane;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* SecondPlane;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* AreaBox;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UMaterialInstanceDynamic* FirstPlaneMaterial;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	class UMaterialInstanceDynamic* SecondPlaneMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FColor OriginColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FColor CrossColor;

	UPROPERTY()
	class AItTakesXGameMode* ItTakesXGameMode;

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

	bool bCharacterHasCrossed = false;


public:
};
