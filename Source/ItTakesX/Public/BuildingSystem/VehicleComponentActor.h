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
	class UStaticMeshComponent* Mesh;

	UPROPERTY()
	AActor* CurrentAimingActor;
	UPROPERTY()
	AActor* CurrentHoistingActor;


	// 该零件所有可连接点的 Component 和材质的引用
	UPROPERTY()
	TArray<FConnectionInfo> ConnectionInfoList;
	// UPROPERTY()
	// TArray<class UStaticMeshComponent*> ConnectionComponents;
	// UPROPERTY()
	// TArray<class UMaterialInstanceDynamic*> ConnectionMaterials;

	// 用于吸附父节点的各类信息
	UPROPERTY()
	USceneComponent* CurrentOverlappingComponent;
	UPROPERTY()
	TScriptInterface<IVehicleNode> CurrentOverlappingVehicleNode;

	bool bHaveCurrentNearestConnectionInfo;
	UPROPERTY()
	FConnectionInfo CurrentNearestConnection;
	UPROPERTY()
	FConnectionInfo CurrentNearestOtherConnection;

	UPROPERTY(EditAnywhere)
	float CurrenForceLength;

	bool bIsRunning;

	// UPROPERTY()
	// FVector CurrentPlaceLocation;
	// UPROPERTY()
	// int32 CurrentConnectionIndex;

	UFUNCTION()
	bool InteractWithOverlappingVehicleNode();

protected:
	// 吸附特效
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADottedLazer> AdsorbEffectClass;
	UPROPERTY()
	class ADottedLazer* CurrentAdsorbEffect;

	UPROPERTY()
	TScriptInterface<IVehicleNode> ParentNode;
	UPROPERTY()
	TArray<TScriptInterface<IVehicleNode>> ChildNodes;

	UPROPERTY(VisibleAnywhere, Category = "Vehicle Properties")
	class USphereComponent* AreaSphere;

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

public:
	// UFUNCTION(BlueprintCallable)
	// void AddConnectionComponent(class USceneComponent* Component);

	virtual void OnBeginAiming_Implementation(AActor* OtherActor);
	virtual void OnEndAiming_Implementation(AActor* OtherActor);
	virtual void OnBeginHoisting_Implementation(AActor* OtherActor);
	virtual void OnEndHoisting_Implementation(AActor* OtherActor);
	// virtual void AddChildNode(TScriptInterface<IVehicleNode> ChildNode) override;
	virtual bool AttachToCurrentOverlappingVehicleNode() override;
	virtual bool IsHoisting() const override;
	virtual TArray<FConnectionInfo> GetConnectionInfoList() override;
	virtual bool
	GetNearestConnectionInfo(FConnectionInfo& OutConnectionInfo, FConnectionInfo& OutOtherConnectionInfo) override;
	virtual bool PropagateCommand(FVehicleCoreCommand Command) override;
	virtual bool AddChildNode(TScriptInterface<IVehicleNode> VehicleNode) override;

	ADottedLazer* SpawnNewAdsorbEffect();
};
