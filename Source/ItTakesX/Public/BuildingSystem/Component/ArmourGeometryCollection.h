// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Field/FieldSystemActor.h"
#include "GameFramework/Actor.h"
#include "ArmourGeometryCollection.generated.h"

UCLASS()
class ITTAKESX_API AArmourGeometryCollection : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AArmourGeometryCollection();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UGeometryCollectionComponent* BodyGeometryCollection;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AFieldSystemActor> FieldSystemClass;
public:
};
