// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Environment/HoistableActor.h"
#include "Interface/VehicleNode.h"
#include "VehicleComponentActor.generated.h"

class UVehicleConnectionComponent;

UCLASS()
class ITTAKESX_API AVehicleComponentActor : public AHoistableActor, public IVehicleNode
{
	GENERATED_BODY()

public:
	AVehicleComponentActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	// 该零件所有可连接点的数据
	UPROPERTY()
	TArray<UVehicleConnectionComponent*> VehicleConnectionComponentList;

	// 用于吸附父节点的各类信息
	// UPROPERTY()
	// USceneComponent* CurrentOverlappingComponent;
	UPROPERTY()
	TArray<TScriptInterface<IVehicleNode>> CurrentOverlappingVehicleNodes;

	bool bHaveCurrentNearestConnectionInfo;
	// UPROPERTY()
	// FConnectionInfo CurrentNearestConnection;
	// UPROPERTY()
	// FConnectionInfo CurrentNearestOtherConnection;
	UPROPERTY()
	UVehicleConnectionComponent* CurrentNearestConnectionComponent;
	UPROPERTY()
	UVehicleConnectionComponent* CurrentNearestOtherConnectionComponent;
	UPROPERTY()
	TScriptInterface<IVehicleNode> CurrenNearestVehicleNode;


	UFUNCTION()
	bool InteractWithOverlappingVehicleNode();

	void AddNewNodeToVehicleNodeList(IVehicleNode* VehicleNode, AActor* Actor);
	void RemoveNodeFromVehicleNodeList(IVehicleNode* VehicleNode);

protected:
	UPROPERTY(VisibleAnywhere, Category = "Vehicle Properties")
	class UStaticMeshComponent* Mesh;

	bool bIsRunning;
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

	UPROPERTY()
	class APhysicsConstraintActor* CurrentConnectionConstraintActor;

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

	virtual void OnBeginHoisting_Implementation(AActor* OtherActor) override;
	virtual void OnEndHoisting_Implementation(AActor* OtherActor) override;

	// virtual void AddChildNode(TScriptInterface<IVehicleNode> ChildNode) override;
	virtual bool AttachToCurrentOverlappingVehicleNode() override;

	virtual TArray<UVehicleConnectionComponent*> GetConnectionInfoList() override;
	virtual bool GetNearestConnectionInfo(UVehicleConnectionComponent*& OutConnectionComponent,
	                                      UVehicleConnectionComponent*&
	                                      OutOtherConnectionComponent,
	                                      TScriptInterface<IVehicleNode>&
	                                      OutNearestVehicleNode) override;
	// virtual bool PropagateCommand(FVehicleCoreCommand Command) override;
	virtual bool AddChildNode(TScriptInterface<IVehicleNode> VehicleNode) override;
	virtual bool RemoveChildNode(TScriptInterface<IVehicleNode> VehicleNode) override;
	virtual void SetIsRunning(bool bNewIsRunning) override;

	virtual TScriptInterface<IVehicleNode> GetVehicleRoot() override;
	virtual TArray<TScriptInterface<IVehicleNode>> GetAllChildNodes() override;

	ADottedLazer* SpawnNewAdsorbEffect();

	virtual UStaticMeshComponent* GetRootMesh() override;
	virtual bool DetachFromParentVehicleNode() override;

	virtual void TurnOnVehicleComponentCollisionChannel() override;
	virtual bool TryTurnOffVehicleComponentCollisionChannel() override;
};
