// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/VehicleControllerActor.h"

#include "BuildingSystem/Component/VehicleComponentSuspensionWheel.h"
#include "BuildingSystem/Component/VehicleComponentThruster.h"
#include "Character/ItTakesXCharacter.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

AVehicleControllerActor::AVehicleControllerActor()
{
	CharacterEnterMark = CreateDefaultSubobject<UArrowComponent>("CharacterEnterMark");
	CharacterExistMark = CreateDefaultSubobject<UArrowComponent>("CharacterExistMark");
	CharacterEnterMark->SetupAttachment(RootComponent);
	CharacterExistMark->SetupAttachment(RootComponent);

	AnchorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AnchorMesh"));
	AnchorMesh->SetupAttachment(RootComponent);

	AnchorConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("AnchorConstraint"));
	AnchorConstraint->SetupAttachment(RootComponent);

	AnchorConstraint->SetConstrainedComponents(Mesh, NAME_None, AnchorMesh, NAME_None);
}

void AVehicleControllerActor::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& SweepResult)
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

void AVehicleControllerActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
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

bool AVehicleControllerActor::IsVehicleStartup() const
{
	return bIsRunning;
}

bool AVehicleControllerActor::StartupVehicle()
{
	auto RootActor = Cast<AActor>(GetVehicleRoot().GetInterface());
	if (RootActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		                                 FString::Printf(
			                                 TEXT("root name %s, tree nodes num %d"), *RootActor->GetName(),
			                                 Cast<IVehicleNode>(RootActor)->GetAllChildNodes().Num()));
		CurrentVehicleNodes = Cast<IVehicleNode>(RootActor)->GetAllChildNodes();
	}

	bIsRunning = true;
	for (auto ChildNodeInterface : CurrentVehicleNodes)
	{
		auto ChildNode = ChildNodeInterface.GetInterface();
		if (ChildNode)
		{
			ChildNode->SetIsRunning(true);
		}
	}

	return true;
}

bool AVehicleControllerActor::ShutdownVehicle()
{
	bIsRunning = false;
	for (auto ChildNodeInterface : CurrentVehicleNodes)
	{
		auto ChildNode = ChildNodeInterface.GetInterface();
		if (ChildNode)
		{
			ChildNode->SetIsRunning(false);
		}
	}

	CurrentVehicleNodes.Reset();

	return true;
}

void AVehicleControllerActor::AttachCharacter(ACharacter* Character)
{
	if (Character == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		                                 FString::Printf(
			                                 TEXT("Character is null")));
		return;
	}

	UPrimitiveComponent* CharacterRootComp = Cast<UPrimitiveComponent>(Character->GetRootComponent());
	if (CharacterRootComp == nullptr)
	{
		return;
	}

	CurrenAttachCharacter = Character;

	CurrenAttachCharacter->GetMovementComponent()->Deactivate();

	CurrenAttachCharacter->SetActorLocation(CharacterEnterMark->GetComponentLocation());
	CurrenAttachCharacter->SetActorRotation(CharacterEnterMark->GetComponentRotation());

	CurrenAttachCharacter->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

void AVehicleControllerActor::DetachCurrentCharacter()
{
	if (CurrenAttachCharacter)
	{
		CurrenAttachCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		CurrenAttachCharacter = nullptr;
	}
}

void AVehicleControllerActor::Throttle(float Value)
{
	for (auto VehicleNodeInterface : CurrentVehicleNodes)
	{
		auto VehicleNode = VehicleNodeInterface.GetInterface();
		auto VehicleSuspensionWheel = Cast<AVehicleComponentSuspensionWheel>(VehicleNode);
		if (VehicleSuspensionWheel)
		{
			VehicleSuspensionWheel->Throttle(Value, GetActorForwardVector(), GetActorRightVector());
		}
	}
}

void AVehicleControllerActor::Turn(float Value)
{
	for (auto VehicleNodeInterface : CurrentVehicleNodes)
	{
		auto VehicleNode = VehicleNodeInterface.GetInterface();
		auto VehicleSuspensionWheel = Cast<AVehicleComponentSuspensionWheel>(VehicleNode);
		if (VehicleSuspensionWheel)
		{
			VehicleSuspensionWheel->Turn(Value, GetActorTransform());
		}
	}
}

void AVehicleControllerActor::AircraftThrottle(float Value)
{
	for (auto VehicleNodeInterface : CurrentVehicleNodes)
	{
		auto VehicleNode = VehicleNodeInterface.GetInterface();
		auto VehicleComponentThruster = Cast<AVehicleComponentThruster>(VehicleNode);
		if (VehicleComponentThruster)
		{
			VehicleComponentThruster->Throttle(Value);
		}
	}
}

void AVehicleControllerActor::AircraftTurn(float Value)
{
	Mesh->AddTorqueInRadians(Mesh->GetUpVector() * AirplaneYawStrength * -Value, NAME_None, true);
	Mesh->AddTorqueInRadians(Mesh->GetForwardVector() * AirplaneRollStrength * Value, NAME_None, true);
}

void AVehicleControllerActor::AircraftPitch(float Value)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(
		                                 TEXT("pitching: %s"),
		                                 *(Mesh->GetRightVector() * AirplanePitchStrength * Value).ToString()));
	Mesh->AddTorqueInRadians(Mesh->GetRightVector() * AirplanePitchStrength * Value, NAME_None, true);
}

void AVehicleControllerActor::SetIsRunning(bool bNewIsRunning)
{
	Super::SetIsRunning(bNewIsRunning);

	if (bNewIsRunning)
	{
		AnchorConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
		AnchorConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Limited, 60.f);
		AnchorConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Limited, 60.f);
		AnchorConstraint->SetAngularDriveParams(BalanceTargetStrength, BalanceVelocityStrength, 0);
	}
	else
	{
		AnchorConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 60.f);
		AnchorConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 60.f);
		AnchorConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 60.f);
		AnchorConstraint->SetAngularDriveParams(0, 0, 0);
	}
}
