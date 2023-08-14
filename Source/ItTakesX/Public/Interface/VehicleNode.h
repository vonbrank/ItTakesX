// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VehicleNode.generated.h"

USTRUCT()
struct FConnectionInfo
{
	GENERATED_BODY()

	UPROPERTY()
	class UStaticMeshComponent* Mesh;
	UPROPERTY()
	class UMaterialInstanceDynamic* Material;
	UPROPERTY()
	class UArrowComponent* Arrow;

	bool operator==(const FConnectionInfo& Rhs) const
	{
		return this->Mesh == Rhs.Mesh && this->Material == Rhs.Material && this->Arrow == Rhs.Arrow;
	}
};

USTRUCT()
struct FVehicleCoreCommand
{
	GENERATED_BODY()
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UVehicleNode : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ITTAKESX_API IVehicleNode
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// virtual void AddChildNode(TScriptInterface<IVehicleNode> ChildNode) = 0;
	virtual bool AttachToCurrentOverlappingVehicleNode() = 0;
	virtual TArray<class UVehicleConnectionComponent*> GetConnectionInfoList() = 0;
	virtual bool GetNearestConnectionInfo(UVehicleConnectionComponent*& OutConnectionComponent,
	                                      UVehicleConnectionComponent*&
	                                      OutOtherConnectionComponent,
	                                      TScriptInterface<IVehicleNode>&
	                                      OutNearestVehicleNode) =
	0;
	// virtual bool PropagateCommand(FVehicleCoreCommand Command) = 0;
	virtual bool AddChildNode(TScriptInterface<IVehicleNode> VehicleNode) = 0;
	virtual bool RemoveChildNode(TScriptInterface<IVehicleNode> VehicleNode) = 0;
	virtual void SetIsRunning(bool bNewIsRunning) = 0;
	virtual TScriptInterface<IVehicleNode> GetVehicleRoot() = 0;
	virtual TArray<TScriptInterface<IVehicleNode>> GetAllChildNodes() = 0;
	virtual bool DetachFromParentVehicleNode() = 0;
	virtual  class UStaticMeshComponent* GetRootMesh() = 0;
};
