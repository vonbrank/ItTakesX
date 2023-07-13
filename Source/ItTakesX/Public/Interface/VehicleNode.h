// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VehicleNode.generated.h"

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
	virtual void AddChildNode(TScriptInterface<IVehicleNode> ChildNode) = 0;
	virtual bool AttachToCurrentOverlappingVehicleNode() = 0;
	virtual bool IsHoisting() const = 0;
	virtual void NearestConnection(FVector SourceLocation, int32& OutConnectionIndex, FVector& OutConnectionLocation,
	                               FVector& OutPlaceLocation) const =
	0;
	virtual void GetBothWayConnectionInfo(const TArray<FVector>& SourceAnchorLocations,
	                                      const TArray<FVector>& SourceArrowLocations,
	                                      const TArray<FRotator>& SourceArrowRotation,
	                                      int32& OutSourceConnectionIndex,
	                                      int32& OutTargetConnectionIndex,
	                                      FVector& OutConnectionLocation,
	                                      FVector& OutPlaceLocation
	) = 0;
	virtual void ActivateConnection(int32 ConnectionIndex) = 0;
	virtual void DeactivateAllConnection() = 0;
};
