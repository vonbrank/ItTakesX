// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Aimable.h"
#include "Interface/Hoistable.h"
#include "Interface/VehicleNode.h"
#include "VehicleComponentActor.generated.h"

UCLASS()
class ITTAKESX_API AVehicleComponentActor : public AActor, public IAimable, public IHoistable, public IVehicleNode
{
	GENERATED_BODY()

public:
	AVehicleComponentActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Vehicle Properties")
	class USphereComponent* AreaSphere;

	UPROPERTY(VisibleAnywhere, Category = "Vehicle Properties")
	class UStaticMeshComponent* Mesh;

	UPROPERTY()
	AActor* CurrentAimingActor;
	UPROPERTY()
	AActor* CurrentHoistingActor;
	UPROPERTY()
	USceneComponent* CurrentOverlappingComponent;

	UPROPERTY()
	TScriptInterface<IVehicleNode> CurrentOverlappingVehicleNode;

	UPROPERTY()
	TScriptInterface<IVehicleNode> ParentNode;
	UPROPERTY()
	TArray<TScriptInterface<IVehicleNode>> ChildNodes;

	UPROPERTY()
	TArray<class UStaticMeshComponent*> ConnectionComponents;
	UPROPERTY()
	TArray<class UMaterialInstanceDynamic*> ConnectionMaterials;

	UPROPERTY()
	FVector CurrentPlaceLocation;
	UPROPERTY()
	int32 CurrentConnectionIndex;

	UFUNCTION()
	virtual void OnSphereStartOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	virtual void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION()
	bool InteractWithOverlappingVehicleNode();


public:
	UFUNCTION(BlueprintCallable)
	void AddConnectionComponent(class USceneComponent* Component);

	virtual void OnBeginAiming_Implementation(AActor* OtherActor);
	virtual void OnEndAiming_Implementation(AActor* OtherActor);
	virtual void OnBeginHoisting_Implementation(AActor* OtherActor);
	virtual void OnEndHoisting_Implementation(AActor* OtherActor);
	virtual void AddChildNode(TScriptInterface<IVehicleNode> ChildNode) override;
	virtual bool AttachToCurrentOverlappingVehicleNode() override;
	virtual bool IsHoisting() const override;
	virtual void NearestConnection(FVector SourceLocation, int32& OutConnectionIndex, FVector& OutConnectionLocation,
	                               FVector& OutPlaceLocation) const override;
	virtual void ActivateConnection(int32 ConnectionIndex) override;
	virtual void DeactivateAllConnection() override;
};
