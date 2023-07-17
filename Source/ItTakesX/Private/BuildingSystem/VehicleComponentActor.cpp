// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/VehicleComponentActor.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
#include "Effect/DottedLazer.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsConstraintActor.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

// Sets default values
AVehicleComponentActor::AVehicleComponentActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("VehicleMesh");
	SetRootComponent(Mesh);
	Mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
	AreaSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	AreaSphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	AreaSphere->ComponentTags.Add(TEXT("VehicleArea"));
	// AreaSphere->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
}

// Called when the game starts or when spawned
void AVehicleComponentActor::BeginPlay()
{
	Super::BeginPlay();

	Mesh->SetSimulatePhysics(true);

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereStartOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);


	TArray<USceneComponent*> AllComponents;
	RootComponent->GetChildrenComponents(true, AllComponents);
	for (auto Comp : AllComponents)
	{
		if (!Comp->ComponentTags.Contains(TEXT("ConnectionComp")))
		{
			continue;
		}
		auto StaticMeshComp = Cast<UStaticMeshComponent>(Comp);
		if (StaticMeshComp == nullptr)
		{
			continue;
		}
		auto Material = StaticMeshComp->CreateDynamicMaterialInstance(0);
		if (Material == nullptr)
		{
			continue;
		}
		UArrowComponent* ArrowComponent = Cast<UArrowComponent>(StaticMeshComp->GetChildComponent(0));
		if (ArrowComponent == nullptr)
		{
			continue;
		}
		FConnectionInfo ConnectionInfo;
		ConnectionInfo.Mesh = StaticMeshComp;
		ConnectionInfo.Material = Material;
		ConnectionInfo.Arrow = ArrowComponent;
		ConnectionInfoList.Add(ConnectionInfo);
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(
		                                 TEXT("name: %s, connection num: %d"), *GetName(), ConnectionInfoList.Num()));
}

// Called every frame
void AVehicleComponentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	InteractWithOverlappingVehicleNode();
}

void AVehicleComponentActor::OnBeginAiming_Implementation(AActor* OtherActor)
{
	CurrentAimingActor = OtherActor;
}

void AVehicleComponentActor::OnEndAiming_Implementation(AActor* OtherActor)
{
	CurrentAimingActor = nullptr;
}

void AVehicleComponentActor::OnBeginHoisting_Implementation(AActor* OtherActor)
{
	CurrentHoistingActor = OtherActor;
	Mesh->SetSimulatePhysics(false);
}

void AVehicleComponentActor::OnEndHoisting_Implementation(AActor* OtherActor)
{
	CurrentHoistingActor = nullptr;
	Mesh->SetSimulatePhysics(true);
}

void AVehicleComponentActor::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	auto VehicleNode = Cast<IVehicleNode>(OtherActor);

	if (CurrentHoistingActor != nullptr && // 必须被举着
		VehicleNode != nullptr && // 另一个 Actor 必须是一个 IVehicleNode
		CurrentOverlappingComponent == nullptr && // 当前没有 Overlapping 的 Component
		OtherComp // 另一个 Component 需要是连接点的 Comp
		->ComponentTags.Contains(
			TEXT("VehicleArea")))
	{
		CurrentOverlappingComponent = OtherComp;
		CurrentOverlappingVehicleNode.SetObject(OtherActor);
		CurrentOverlappingVehicleNode.SetInterface(VehicleNode);

		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Yellow,
			FString::Printf(
				TEXT("Start overlap: %s, owner: %s"), *OtherComp->GetName(), *GetName()));
	}
}

void AVehicleComponentActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp == CurrentOverlappingComponent)
	{
		CurrentOverlappingComponent = nullptr;
		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Yellow,
			FString::Printf(
				TEXT("End overlap: %s, owner: %s"), *OtherComp->GetName(), *GetName()));

		auto VehicleNode = CurrentOverlappingVehicleNode.GetInterface();

		CurrentOverlappingVehicleNode.SetObject(nullptr);
		CurrentOverlappingVehicleNode.SetInterface(nullptr);
	}
}

bool AVehicleComponentActor::AttachToCurrentOverlappingVehicleNode()
{
	if (!bHaveCurrentNearestConnectionInfo) return false;

	auto ParentActor = Cast<AActor>(CurrentOverlappingVehicleNode.GetInterface());

	if (ParentActor == nullptr) return false;
	auto ParentRootComponent = Cast<UPrimitiveComponent>(ParentActor->GetRootComponent());
	if (ParentRootComponent == nullptr)
	{
		return false;
	}
	// SetActorLocation(CurrentNearestConnection.Arrow->GetComponentLocation());
	FVector DeltaLocation = CurrentNearestOtherConnection.Arrow->GetComponentLocation()
		- CurrentNearestConnection.Arrow->GetComponentLocation();
	AddActorWorldOffset(DeltaLocation);

	FVector MidLocation = CurrentNearestConnection.Mesh->GetComponentLocation() + (CurrentNearestOtherConnection.Mesh->
		GetComponentLocation() - CurrentNearestConnection.Mesh->GetComponentLocation()) / 2;

	ParentRootComponent->SetSimulatePhysics(false);
	Mesh->SetSimulatePhysics(false);

	APhysicsConstraintActor* PhysicsConstraintActor = GetWorld()->SpawnActor<APhysicsConstraintActor>();
	PhysicsConstraintActor->SetActorLocation(MidLocation);
	UPhysicsConstraintComponent* PhysicsConstraintComponent = PhysicsConstraintActor->GetConstraintComp();
	PhysicsConstraintComponent->SetDisableCollision(true);
	PhysicsConstraintComponent->SetConstrainedComponents(Mesh, TEXT(""), ParentRootComponent, TEXT(""));
	PhysicsConstraintComponent->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	PhysicsConstraintComponent->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	PhysicsConstraintComponent->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
	//
	ParentRootComponent->SetSimulatePhysics(true);
	Mesh->SetSimulatePhysics(true);

	return true;
}

bool AVehicleComponentActor::IsHoisting() const
{
	return CurrentHoistingActor != nullptr;
}

bool AVehicleComponentActor::InteractWithOverlappingVehicleNode()
{
	//TODO Waiting for optimizing action between this and other vehicle node

	auto VehicleNode = CurrentOverlappingVehicleNode.GetInterface();


	if (CurrentHoistingActor == nullptr || VehicleNode == nullptr)
	{
		if (CurrentAdsorbEffect)
		{
			CurrentAdsorbEffect->Destroy();
			CurrentAdsorbEffect = nullptr;
		}
		return false;
	}

	FConnectionInfo OutCurrentConnectionInfo;
	FConnectionInfo OutCurrentOtherConnectionInfo;
	bHaveCurrentNearestConnectionInfo = GetNearestConnectionInfo(OutCurrentConnectionInfo,
	                                                             OutCurrentOtherConnectionInfo);


	if (bHaveCurrentNearestConnectionInfo)
	{
		if (OutCurrentConnectionInfo == CurrentNearestConnection && OutCurrentOtherConnectionInfo ==
			CurrentNearestOtherConnection)
		{
			if (CurrentAdsorbEffect)
			{
				CurrentAdsorbEffect->SetEndLocation(OutCurrentOtherConnectionInfo.Mesh->GetComponentLocation());
			}
			else
			{
				SpawnNewAdsorbEffect();
			}
		}
		else
		{
			CurrentNearestConnection = OutCurrentConnectionInfo;
			CurrentNearestOtherConnection = OutCurrentOtherConnectionInfo;
			SpawnNewAdsorbEffect();
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		                                 FString::Printf(TEXT("Same Connection, effect: %p"), CurrentAdsorbEffect));
	}
	else
	{
		if (CurrentAdsorbEffect)
		{
			CurrentAdsorbEffect->Destroy();
			CurrentAdsorbEffect = nullptr;
		}
	}

	return true;
}

TArray<FConnectionInfo> AVehicleComponentActor::GetConnectionInfoList()
{
	return ConnectionInfoList;
}

bool AVehicleComponentActor::GetNearestConnectionInfo(FConnectionInfo& OutConnectionInfo,
                                                      FConnectionInfo& OutOtherConnectionInfo)
{
	auto VehicleNode = CurrentOverlappingVehicleNode.GetInterface();
	if (VehicleNode == nullptr)
	{
		return false;
	}

	bool bHaveAnswer = false;
	float MinDistance = TNumericLimits<float>::Max();

	for (auto OtherConnectionInfo : VehicleNode->GetConnectionInfoList())
	{
		for (auto ConnectionInfo : ConnectionInfoList)
		{
			auto OtherMesh = OtherConnectionInfo.Mesh;
			auto ThisMesh = ConnectionInfo.Mesh;
			if (OtherMesh == nullptr || ThisMesh == nullptr)
			{
				continue;
			}

			auto CurrentDistance = (OtherMesh->GetComponentLocation() - ThisMesh->GetComponentLocation()).Length();

			if (CurrentDistance < MinDistance)
			{
				MinDistance = CurrentDistance;
				OutConnectionInfo = ConnectionInfo;
				OutOtherConnectionInfo = OtherConnectionInfo;
				bHaveAnswer = true;
			}
		}
	}

	return bHaveAnswer;
}

ADottedLazer* AVehicleComponentActor::SpawnNewAdsorbEffect()
{
	if (!bHaveCurrentNearestConnectionInfo)
	{
		return nullptr;
	}

	if (CurrentAdsorbEffect)
	{
		CurrentAdsorbEffect->Destroy();
		CurrentAdsorbEffect = nullptr;
	}

	CurrentAdsorbEffect = GetWorld()->SpawnActor<ADottedLazer>(
		AdsorbEffectClass, CurrentNearestConnection.Mesh->GetComponentLocation(), FRotator::ZeroRotator);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(TEXT("Spawn result: %p"), CurrentAdsorbEffect));

	if (CurrentAdsorbEffect)
	{
		CurrentAdsorbEffect->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}

	return CurrentAdsorbEffect;
}
