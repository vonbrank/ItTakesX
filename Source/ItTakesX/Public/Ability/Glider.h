// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Equippable.h"
#include "Glider.generated.h"

UCLASS()
class ITTAKESX_API AGlider : public AActor, public IEquippable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGlider();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
private:
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float RotatingSpeed = 180.f;

protected:
public:
	virtual EItTakesXViewType GetViewType() override;
	virtual float GetMaxWalkingSpeed() override;
};
