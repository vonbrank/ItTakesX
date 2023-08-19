// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/VehicleControllerActor.h"

#include "BuildingSystem/Component/VehicleComponentArmour.h"
#include "BuildingSystem/Component/VehicleComponentFlameThrower.h"
#include "BuildingSystem/Component/VehicleComponentSuspensionWheel.h"
#include "BuildingSystem/Component/VehicleComponentThruster.h"
#include "BuildingSystem/Component/VehicleComponentTurret.h"
#include "Character/ItTakesXCharacter.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameModes/ItTakesXGameMode.h"
#include "Kismet/GameplayStatics.h"
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

void AVehicleControllerActor::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;

	if (DamageRatioTransferToArmour < 0)
	{
		DamageRatioTransferToArmour = 0;
	}
	else if (DamageRatioTransferToArmour > 1)
	{
		DamageRatioTransferToArmour = 1;
	}

	ItTakesXGameMode = Cast<AItTakesXGameMode>(UGameplayStatics::GetGameMode(this));

	InitialTransform = GetTransform();
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

void AVehicleControllerActor::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                          AController* DamageInstigator, AActor* DamageCauser)
{
	// 调用父类同名函数将导致递归调用
	// Super::DamageTaken(DamagedActor, Damage, DamageType, DamageInstigator, DamageCauser);

	if (Cast<IVehicleNode>(DamageCauser) || DamageInstigator == GetWorld()->GetFirstPlayerController())
	{
		return;
	}

	if (Health <= 0)
	{
		return;
	}


	if (CurrentVehicleComponentArmours.Num() > 0)
	{
		float DamageToArmours = Damage * DamageRatioTransferToArmour / CurrentVehicleComponentArmours.Num();
		float DamageToSelf = Damage * (1 - DamageRatioTransferToArmour);

		for (auto VehicleComponentArmour : CurrentVehicleComponentArmours)
		{
			VehicleComponentArmour->DamageTaken(DamagedActor, DamageToArmours, DamageType, DamageInstigator,
			                                    DamageCauser);
		}
		Health -= DamageToSelf;
	}
	else
	{
		Health -= Damage;
	}

	if (Health <= 0)
	{
		DestroyVehicle();
	}
}

void AVehicleControllerActor::UpdateArmourHealth()
{
	CurrentArmourHealth = 0;
	for (auto VehicleComponentArmour
	     :
	     CurrentVehicleComponentArmours
	)
	{
		if (VehicleComponentArmour)
		{
			CurrentArmourHealth += VehicleComponentArmour->GetHealth();
		}
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("current armourhealth / armourmaxhealth = %.f/%.f"), CurrentArmourHealth,
	// 	                                 CurrentMaxArmourHealth));
}

void AVehicleControllerActor::RebootVehicle()
{
	auto PreviousAirplaneThrottle = CurrentAirplaneThrottle;
	ShutdownVehicle();
	StartupVehicle();
	CurrentAirplaneThrottle = PreviousAirplaneThrottle;
}

TScriptInterface<IVehicleNode> AVehicleControllerActor::GetRandomComponentFromVehicle()
{
	if (CurrentVehicleNodes.Num() == 0)
	{
		TScriptInterface<IVehicleNode> Res;
		Res.SetInterface(nullptr);
		Res.SetObject(nullptr);
		return Res;
	}

	auto Index = FMath::RandRange(0, CurrentVehicleNodes.Num() - 1);
	return CurrentVehicleNodes[Index];
}

void AVehicleControllerActor::DestroyVehicle()
{
	auto PreviousVehicleNodes = CurrentVehicleNodes;

	VehicleDestroyDelegate.Broadcast();
	VehicleDestroyDelegate.Clear();

	for (auto VehicleNode : PreviousVehicleNodes)
	{
		if (VehicleNode.GetInterface())
		{
			VehicleNode->DetachFromParentVehicleNode();
		}
	}

	GetWorld()->SpawnActor<AActor>(GeometryCollectionActorClass, GetActorLocation(), GetActorRotation());

	SetActorTransform(InitialTransform);
}

bool AVehicleControllerActor::IsVehicleStartup() const
{
	return bIsRunning;
}

bool AVehicleControllerActor::StartupVehicle(bool bNeedUpdateUsedComponents)
{
	auto RootNode = GetVehicleRoot();
	auto RootActor = Cast<AActor>(RootNode.GetInterface());
	if (RootActor == nullptr)
	{
		return false;
	}

	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("root name %s, tree nodes num %d"), *RootActor->GetName(),
	// 	                                 Cast<IVehicleNode>(RootActor)->GetAllChildNodes().Num()));
	CurrentVehicleNodes = Cast<IVehicleNode>(RootActor)->GetAllChildNodes();

	bIsRunning = true;

	TScriptInterface<IVehicleNode> VehicleController;
	VehicleController.SetInterface(this);
	VehicleController.SetObject(this);

	for (auto ChildNodeInterface : CurrentVehicleNodes)
	{
		auto ChildNode = ChildNodeInterface.GetInterface();
		if (ChildNode)
		{
			ChildNode->SetIsRunning(true);
			ChildNode->SetCurrentRunningRoot(RootNode);
			ChildNode->SetCurrentRunningVehicleController(VehicleController);
		}

		auto VehicleComponentSuspension = Cast<AVehicleComponentSuspensionWheel>(ChildNode);
		auto VehicleComponentThruster = Cast<AVehicleComponentThruster>(ChildNode);
		auto VehicleComponentFlameThrower = Cast<AVehicleComponentFlameThrower>(ChildNode);
		auto VehicleComponentTurret = Cast<AVehicleComponentTurret>(ChildNode);
		auto VehicleComponentArmour = Cast<AVehicleComponentArmour>(ChildNode);

		if (VehicleComponentSuspension)
		{
			CurrentVehicleComponentSuspensionWheels.Add(VehicleComponentSuspension);
		}
		else if (VehicleComponentThruster)
		{
			CurrentVehicleComponentThrusters.Add(VehicleComponentThruster);
		}
		else if (VehicleComponentFlameThrower)
		{
			CurrentVehicleComponentFlameThrowers.Add(VehicleComponentFlameThrower);
		}
		else if (VehicleComponentTurret)
		{
			CurrentVehicleComponentTurrets.Add(VehicleComponentTurret);
		}
		else if (VehicleComponentArmour)
		{
			CurrentVehicleComponentArmours.Add(VehicleComponentArmour);
		}
	}
	{
		CurrentMaxArmourHealth = 0;
		for (auto VehicleComponentArmour
		     :
		     CurrentVehicleComponentArmours
		)
		{
			if (VehicleComponentArmour)
			{
				CurrentMaxArmourHealth += VehicleComponentArmour->GetHealth();
			}
		}
		CurrentArmourHealth = CurrentMaxArmourHealth;

		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(
		// 	                                 TEXT("root name %s, tree nodes num %d"), *RootActor->GetName(),
		// 	                                 Cast<IVehicleNode>(RootActor)->GetAllChildNodes().Num()));
	}

	if (bNeedUpdateUsedComponents && ItTakesXGameMode)
	{
		ItTakesXGameMode->NewUseVehicleComponents(CurrentVehicleNodes);
	}

	return true;
}

bool AVehicleControllerActor::ShutdownVehicle()
{
	bIsRunning = false;

	TScriptInterface<IVehicleNode> EmptyVehicleNode;
	EmptyVehicleNode.SetInterface(nullptr);
	EmptyVehicleNode.SetObject(nullptr);

	for (auto ChildNodeInterface : CurrentVehicleNodes)
	{
		auto ChildNode = ChildNodeInterface.GetInterface();
		if (ChildNode)
		{
			ChildNode->SetIsRunning(false);
			ChildNode->SetCurrentRunningRoot(EmptyVehicleNode);
			ChildNode->SetCurrentRunningVehicleController(EmptyVehicleNode);
		}
	}

	CurrentVehicleNodes.Reset();
	CurrentVehicleComponentSuspensionWheels.Reset();
	CurrentVehicleComponentThrusters.Reset();
	CurrentVehicleComponentFlameThrowers.Reset();
	CurrentVehicleComponentTurrets.Reset();
	CurrentVehicleComponentArmours.Reset();

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
	for (auto VehicleSuspensionWheel : CurrentVehicleComponentSuspensionWheels)
	{
		if (VehicleSuspensionWheel)
		{
			VehicleSuspensionWheel->Throttle(Value, GetActorForwardVector(), GetActorRightVector());
		}
	}
}

void AVehicleControllerActor::Turn(float Value)
{
	for (auto VehicleSuspensionWheel : CurrentVehicleComponentSuspensionWheels)
	{
		if (VehicleSuspensionWheel)
		{
			VehicleSuspensionWheel->Turn(Value, GetActorTransform());
		}
	}
}

void AVehicleControllerActor::AircraftThrottle(float Value)
{
	CurrentAirplaneThrottle += Value * AirplaneThrottleAcceleration * GetWorld()->DeltaTimeSeconds;
	if (CurrentAirplaneThrottle < 0)
	{
		CurrentAirplaneThrottle = 0;
	}
	else if (CurrentAirplaneThrottle > MaxAirplaneThrottle)
	{
		CurrentAirplaneThrottle = MaxAirplaneThrottle;
	}

	for (auto VehicleComponentThruster : CurrentVehicleComponentThrusters)
	{
		if (VehicleComponentThruster)
		{
			VehicleComponentThruster->Throttle(CurrentAirplaneThrottle);
		}
	}
}

void AVehicleControllerActor::AircraftTurn(float Value)
{
	FVector CurrentVelocity = Mesh->GetPhysicsLinearVelocity();
	// FVector HorizontalVelocity = CurrentVelocity + Mesh->GetUpVector() * (FVector::DotProduct(
	// 	CurrentVelocity, Mesh->GetUpVector()) * -1);
	FVector ForwardVelocity = FVector::DotProduct(Mesh->GetForwardVector(), CurrentVelocity) * Mesh->GetForwardVector();
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(TEXT("ForwardVelocity = %f"), ForwardVelocity.Length()));
	Mesh->AddTorqueInRadians(
		Mesh->GetUpVector() * AirplaneYawStrength / FMath::Max(MinYawTurnDampingSpeed, ForwardVelocity.Length()) * -
		Value,
		NAME_None, true);
	Mesh->AddTorqueInRadians(Mesh->GetForwardVector() * AirplaneRollStrength * Value, NAME_None, true);
}

void AVehicleControllerActor::AircraftPitch(float Value)
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("pitching: %s"),
	// 	                                 *(Mesh->GetRightVector() * AirplanePitchStrength * Value).ToString()));
	Mesh->AddTorqueInRadians(Mesh->GetRightVector() * AirplanePitchStrength * Value, NAME_None, true);
}

void AVehicleControllerActor::ToggleOpenFire()
{
	for (auto VehicleComponentFlameThrower : CurrentVehicleComponentFlameThrowers)
	{
		if (VehicleComponentFlameThrower)
		{
			VehicleComponentFlameThrower->ToggleOpenFire();
		}
	}
}

void AVehicleControllerActor::LaunchProjectile()
{
	for (auto VehicleComponentTurret : CurrentVehicleComponentTurrets)
	{
		if (VehicleComponentTurret)
		{
			VehicleComponentTurret->LaunchProjectile();
		}
	}
}

void AVehicleControllerActor::VerticalRotateTurret(float Value)
{
	if (bAimingOpenFireMode)
	{
		return;
	}
	for (auto VehicleComponentTurret : CurrentVehicleComponentTurrets)
	{
		if (VehicleComponentTurret)
		{
			VehicleComponentTurret->VerticalRotateTurret(Value);
		}
	}
}

void AVehicleControllerActor::HorizontalRotateTurret(float Value)
{
	if (bAimingOpenFireMode)
	{
		return;
	}
	for (auto VehicleComponentTurret : CurrentVehicleComponentTurrets)
	{
		if (VehicleComponentTurret)
		{
			VehicleComponentTurret->HorizontalRotateTurret(Value);
		}
	}
}

void AVehicleControllerActor::TurretLookAtTarget(FVector TargetLocation)
{
	if (!bAimingOpenFireMode)
	{
		return;
	}
	for (auto VehicleComponentTurret : CurrentVehicleComponentTurrets)
	{
		if (VehicleComponentTurret)
		{
			VehicleComponentTurret->LookAtTarget(TargetLocation);
		}
	}
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
		CurrentAirplaneThrottle = 0;
	}
	else
	{
		AnchorConstraint->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Free, 60.f);
		AnchorConstraint->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 60.f);
		AnchorConstraint->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Free, 60.f);
		AnchorConstraint->SetAngularDriveParams(0, 0, 0);
		AircraftThrottle(0);
	}
}
