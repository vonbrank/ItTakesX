// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ArmourGeometryCollection.h"
#include "BuildingSystem/VehicleComponentActor.h"
#include "Field/FieldSystemActor.h"
#include "VehicleComponentArmour.generated.h"

/**
 * 
 */
UCLASS()
class ITTAKESX_API AVehicleComponentArmour : public AVehicleComponentActor
{
	GENERATED_BODY()
public:
	AVehicleComponentArmour();
	virtual void BeginPlay() override;
private:
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BodyMesh75Health;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AArmourGeometryCollection> Destruct75GeometryClass;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BodyMesh50Health;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AArmourGeometryCollection> Destruct50GeometryClass;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BodyMesh25Health;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AArmourGeometryCollection> Destruct25GeometryClass;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AArmourGeometryCollection> Destruct00GeometryClass;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	float MaxHealth = 3000;

	float Health;

	UPROPERTY()
	class AArmourGeometryCollection* CurrentArmourGeometryCollection;

	UPROPERTY(EditAnywhere)
	float ArmourGeometryCollectionLifeSpan = 2.f;

	virtual void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                         AController* DamageInstigator, AActor* DamageCauser) override;

public:
	UFUNCTION(BlueprintCallable)
	void Destruct75();

	UFUNCTION(BlueprintCallable)
	void Destruct50();

	UFUNCTION(BlueprintCallable)
	void Destruct25();

	UFUNCTION(BlueprintCallable)
	void Destruct00();
};
