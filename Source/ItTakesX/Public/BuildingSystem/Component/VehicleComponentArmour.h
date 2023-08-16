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
