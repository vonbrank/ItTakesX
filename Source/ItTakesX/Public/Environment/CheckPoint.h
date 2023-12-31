// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CheckPoint.generated.h"

UCLASS()
class ITTAKESX_API ACheckPoint : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACheckPoint();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ButtonBase;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* ButtonBody;

	UPROPERTY(VisibleAnywhere)
	class UPhysicsConstraintComponent* ButtonConstraint;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* TokenCubeSpawnPoint;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> TokenCubeClass;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* FlagBase;

	UPROPERTY(VisibleAnywhere)
	class UPhysicsConstraintComponent* FlagConstraint;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* FlagBody;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* CharacterRespawnPoint;

	UPROPERTY(EditAnywhere)
	float ActiveThreshold;

	UPROPERTY(EditAnywhere)
	float ActivePressingTime;

	UPROPERTY(EditAnywhere)
	FColor ButtonLightUpColor;

	UPROPERTY(EditAnywhere)
	FColor ButtonLightDownColor;

	UPROPERTY(EditAnywhere)
	FName SpawnCubeTag;

	UPROPERTY()
	class UMaterialInstanceDynamic* ButtonBodyDynamicMaterial;

	UPROPERTY()
	class AActor* CurrentSpawnActor;

	float CurrentPressingTime;
	bool bCurrentHasActive;

	bool bHasRoseFlag = false;

	void SpawnTokenCube();

	void ResetButtonState();
	void LightUpButton();
	void LightDownButton();

	UPROPERTY()
	class AItTakesXGameMode* ItTakesXGameMode;

protected:
	virtual bool CanSpawnTokenCube();
	virtual void RiseFlag();

public:
	FTransform GetCharacterRespawnPointTransform();
};
