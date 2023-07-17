// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/VehicleCoreActor.h"

#include "Character/ItTakesXCharacter.h"

void AVehicleCoreActor::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	Super::OnSphereStartOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	AItTakesXCharacter* Character = Cast<AItTakesXCharacter>(OtherActor);
	if (Character && CurrentOverlappingCharacter == nullptr)
	{
		Character->SetCurrentOverlappingVehicleCore(this);
		CurrentOverlappingCharacter = Character;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Start overlapping character")));
	}
}

void AVehicleCoreActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	if (OtherActor == CurrentOverlappingCharacter)
	{
		CurrentOverlappingCharacter->SetCurrentOverlappingVehicleCore(nullptr);
		CurrentOverlappingCharacter = nullptr;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("End overlapping character")));
	}
}

bool AVehicleCoreActor::PropagateCommand(FVehicleCoreCommand Command)
{
	for (auto ChildNodeInterface : ChildNodes)
	{
		auto ChildNode = ChildNodeInterface.GetInterface();
		if (ChildNode)
		{
			ChildNode->PropagateCommand(Command);
		}
	}
	return true;
}
