// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/VehicleCoreActor.h"

#include "Character/ItTakesXCharacter.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

AVehicleCoreActor::AVehicleCoreActor()
{
	CharacterEnterMark = CreateDefaultSubobject<UArrowComponent>("CharacterEnterMark");
	CharacterExistMark = CreateDefaultSubobject<UArrowComponent>("CharacterExistMark");
	CharacterEnterMark->SetupAttachment(RootComponent);
	CharacterExistMark->SetupAttachment(RootComponent);
}

void AVehicleCoreActor::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                             const FHitResult& SweepResult)
{
	Super::OnSphereStartOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// AItTakesXCharacter* Character = Cast<AItTakesXCharacter>(OtherActor);
	// if (Character && CurrentOverlappingCharacter == nullptr)
	// {
	// 	Character->SetCurrentOverlappingVehicleCore(nullptr);
	// 	CurrentOverlappingCharacter = Character;
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Start overlapping character")));
	// }
}

void AVehicleCoreActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);

	// if (OtherActor == CurrentOverlappingCharacter)
	// {
	// 	CurrentOverlappingCharacter->SetCurrentOverlappingVehicleCore(nullptr);
	// 	CurrentOverlappingCharacter = nullptr;
	// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("End overlapping character")));
	// }
}

bool AVehicleCoreActor::IsVehicleStartup() const
{
	return bIsRunning;
}

bool AVehicleCoreActor::StartupVehicle()
{
	bIsRunning = true;
	for (auto ChildNodeInterface : ChildNodes)
	{
		auto ChildNode = ChildNodeInterface.GetInterface();
		if (ChildNode)
		{
			ChildNode->SetIsRunning(true);
		}
	}
	return true;
}

bool AVehicleCoreActor::ShutdownVehicle()
{
	bIsRunning = false;
	for (auto ChildNodeInterface : ChildNodes)
	{
		auto ChildNode = ChildNodeInterface.GetInterface();
		if (ChildNode)
		{
			ChildNode->SetIsRunning(false);
		}
	}
	return true;
}

void AVehicleCoreActor::AttachCharacter(ACharacter* Character)
{
	if (Character == nullptr)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(
		// 	                                 TEXT("Character is null")));
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

	// CharacterConstraintActor = GetWorld()->SpawnActor<APhysicsConstraintActor>();
	// CharacterConstraintActor->SetActorLocation(GetActorLocation());

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(TEXT("New Constraint: %s"), *CharacterConstraintActor->GetName()));

	// UPhysicsConstraintComponent* PhysicsConstraintComponent = CharacterConstraintActor->GetConstraintComp();
	// PhysicsConstraintComponent->SetDisableCollision(true);
	// PhysicsConstraintComponent->SetConstrainedComponents(CharacterRootComp, TEXT(""), Mesh, TEXT(""));
	//
	// CharacterRootComp->SetSimulatePhysics(true);
	//
	// PhysicsConstraintComponent->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	// PhysicsConstraintComponent->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	// PhysicsConstraintComponent->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
}

void AVehicleCoreActor::DetachCurrentCharacter()
{
	// if (CharacterConstraintActor)
	// {
	// 	CharacterConstraintActor->Destroy();
	// 	CharacterConstraintActor = nullptr;
	// }
	if (CurrenAttachCharacter)
	{
		CurrenAttachCharacter->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		// UPrimitiveComponent* CharacterRootComp = Cast<UPrimitiveComponent>(CurrenAttachCharacter->GetRootComponent());
		// if (CharacterRootComp)
		// {
		// 	CharacterRootComp->SetSimulatePhysics(false);
		// }
		// CurrenAttachCharacter->SetActorLocation(CharacterExistMark->GetComponentLocation());
		// CurrenAttachCharacter->GetMovementComponent()->Activate();
		CurrenAttachCharacter = nullptr;
	}
}
