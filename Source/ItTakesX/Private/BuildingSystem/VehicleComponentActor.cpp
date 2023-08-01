// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/VehicleComponentActor.h"

#include "BuildingSystem/VehicleConnectionComponent.h"
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
		UVehicleConnectionComponent* VehicleConnectionComponent = Cast<UVehicleConnectionComponent>(Comp);
		if (VehicleConnectionComponent)
		{
			VehicleConnectionComponentList.Add(VehicleConnectionComponent);
		}
	}
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(
		                                 TEXT("name: %s, connection num: %d"), *GetName(),
		                                 VehicleConnectionComponentList.Num()));
}

void AVehicleComponentActor::SetIsRunning(bool bNewIsRunning)
{
	bIsRunning = bNewIsRunning;
}

// Called every frame
void AVehicleComponentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (InteractWithOverlappingVehicleNode())
	{
		return;
	}
}


void AVehicleComponentActor::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                  const FHitResult& SweepResult)
{
	auto VehicleNode = Cast<IVehicleNode>(OtherActor);

	if (CurrentHoistingActor != nullptr && // 必须被举着
		VehicleNode != nullptr && // 另一个 Actor 必须是一个 IVehicleNode
		OtherComp // 另一个 Component 需要是连接点的 Comp
		->ComponentTags.Contains(TEXT("VehicleArea")))
	{
		AddNewNodeToVehicleNodeList(VehicleNode, OtherActor);

		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Yellow,
			FString::Printf(
				TEXT("Start overlap: %s, owner: %s, list length: %d"), *OtherComp->GetName(), *OtherActor->GetName(),
				CurrentOverlappingVehicleNodes.Num()));
	}
}

void AVehicleComponentActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto VehicleNode = Cast<IVehicleNode>(OtherActor);
	if (CurrentHoistingActor != nullptr && VehicleNode != nullptr && OtherComp
	                                                                 ->ComponentTags.Contains(TEXT("VehicleArea")))
	// 另一个 Component 需要是连接点的 Comp
	{
		RemoveNodeFromVehicleNodeList(VehicleNode);

		GEngine->AddOnScreenDebugMessage(
			-1, 15.f, FColor::Yellow,
			FString::Printf(
				TEXT("End overlap: %s, owner: %s, list length: %d"), *OtherComp->GetName(), *OtherActor->GetName(),
				CurrentOverlappingVehicleNodes.Num()));
	}
}

void AVehicleComponentActor::AddNewNodeToVehicleNodeList(IVehicleNode* VehicleNode, AActor* Actor)
{
	if (VehicleNode == nullptr || Actor == nullptr)
	{
		return;
	}

	bool bInList = false;
	for (auto NodeInterface : CurrentOverlappingVehicleNodes)
	{
		if (NodeInterface.GetInterface() == VehicleNode)
		{
			bInList = true;
			break;
		}
	}
	if (bInList)
	{
		return;
	}

	TScriptInterface<IVehicleNode> NewNodeInterface;
	NewNodeInterface.SetObject(Actor);
	NewNodeInterface.SetInterface(VehicleNode);

	CurrentOverlappingVehicleNodes.Add(NewNodeInterface);
}

void AVehicleComponentActor::RemoveNodeFromVehicleNodeList(IVehicleNode* VehicleNode)
{
	int Index = -1;

	for (int i = 0; i < CurrentOverlappingVehicleNodes.Num(); i++)
	{
		if (CurrentOverlappingVehicleNodes[i].GetInterface() == VehicleNode)
		{
			Index = i;
			break;
		}
	}
	if (Index == -1)
	{
		return;
	}
	CurrentOverlappingVehicleNodes.RemoveAt(Index);
}

bool AVehicleComponentActor::AttachToCurrentOverlappingVehicleNode()
{
	if (!bHaveCurrentNearestConnectionInfo) return false;

	auto ParentActor = Cast<AActor>(CurrenNearestVehicleNode.GetInterface());

	if (ParentActor == nullptr) return false;
	auto ParentRootComponent = Cast<UPrimitiveComponent>(ParentActor->GetRootComponent());
	if (ParentRootComponent == nullptr)
	{
		return false;
	}
	// SetActorLocation(CurrentNearestConnection.Arrow->GetComponentLocation());
	FVector DeltaLocation = CurrentNearestOtherConnectionComponent->GetDirectionArrowLocation()
		- CurrentNearestConnectionComponent->GetDirectionArrowLocation();
	AddActorWorldOffset(DeltaLocation);

	FVector MidLocation = CurrentNearestConnectionComponent->GetComponentLocation() + (
		CurrentNearestOtherConnectionComponent->
		GetComponentLocation() - CurrentNearestConnectionComponent->GetComponentLocation()) / 2;

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

	ParentNode = CurrenNearestVehicleNode;
	if (ParentNode.GetInterface())
	{
		TScriptInterface<IVehicleNode> SonNode;
		SonNode.SetObject(this);
		SonNode.SetInterface(this);
		ParentNode->AddChildNode(SonNode);
	}

	return true;
}

bool AVehicleComponentActor::InteractWithOverlappingVehicleNode()
{
	//TODO Waiting for optimizing action between this and other vehicle node

	// auto VehicleNode = CurrenNearestVehicleNode.GetInterface();


	if (CurrentHoistingActor == nullptr || CurrentOverlappingVehicleNodes.Num() == 0)
	{
		if (CurrentAdsorbEffect)
		{
			CurrentAdsorbEffect->Destroy();
			CurrentAdsorbEffect = nullptr;
		}
		return false;
	}

	UVehicleConnectionComponent* OutConnectionComponent = nullptr;
	UVehicleConnectionComponent* OutOtherConnectionComponent = nullptr;
	TScriptInterface<IVehicleNode> OutNearestVehicleNode;

	bHaveCurrentNearestConnectionInfo = GetNearestConnectionInfo(OutConnectionComponent,
	                                                             OutOtherConnectionComponent,
	                                                             OutNearestVehicleNode);

	if (bHaveCurrentNearestConnectionInfo)
	{
		if (CurrentNearestConnectionComponent == OutConnectionComponent && CurrentNearestOtherConnectionComponent ==
			OutOtherConnectionComponent && OutNearestVehicleNode.GetInterface() == CurrenNearestVehicleNode.
			GetInterface())
		{
			if (CurrentAdsorbEffect)
			{
				CurrentAdsorbEffect->SetEndLocation(OutOtherConnectionComponent->GetComponentLocation());
			}
			else
			{
				SpawnNewAdsorbEffect();
			}
		}
		else
		{
			CurrentNearestConnectionComponent = OutConnectionComponent;
			CurrentNearestOtherConnectionComponent = OutOtherConnectionComponent;
			CurrenNearestVehicleNode = OutNearestVehicleNode;
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

TArray<UVehicleConnectionComponent*> AVehicleComponentActor::GetConnectionInfoList()
{
	return VehicleConnectionComponentList;
}

bool AVehicleComponentActor::GetNearestConnectionInfo(UVehicleConnectionComponent*& OutConnectionComponent,
                                                      UVehicleConnectionComponent*& OutOtherConnectionComponent,
                                                      TScriptInterface<IVehicleNode>& OutNearestVehicleNode)
{
	bool bHaveAnswer = false;
	float MinDistance = TNumericLimits<float>::Max();

	for (auto VehicleNode : CurrentOverlappingVehicleNodes)
	{
		for (auto OtherVehicleConnectionComponent : VehicleNode->GetConnectionInfoList())
		{
			for (auto VehicleConnectionComponent : VehicleConnectionComponentList)
			{
				auto CurrentDistance = (OtherVehicleConnectionComponent->GetComponentLocation() -
					VehicleConnectionComponent->GetComponentLocation()).Length();

				if (CurrentDistance < MinDistance)
				{
					MinDistance = CurrentDistance;
					OutConnectionComponent = VehicleConnectionComponent;
					OutOtherConnectionComponent = OtherVehicleConnectionComponent;
					OutNearestVehicleNode = VehicleNode;
					bHaveAnswer = true;
				}
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
		AdsorbEffectClass, CurrentNearestConnectionComponent->GetComponentLocation(), FRotator::ZeroRotator);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(TEXT("Spawn result: %p"), CurrentAdsorbEffect));

	if (CurrentAdsorbEffect)
	{
		CurrentAdsorbEffect->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}

	return CurrentAdsorbEffect;
}

// bool AVehicleComponentActor::PropagateCommand(FVehicleCoreCommand Command)
// {
// 	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("%s receives command"), *GetName()));
//
// 	bIsRunning = true;
//
// 	return false;
// }

bool AVehicleComponentActor::AddChildNode(TScriptInterface<IVehicleNode> VehicleNode)
{
	ChildNodes.Add(VehicleNode);

	return true;
}

void AVehicleComponentActor::OnBeginHoisting_Implementation(AActor* OtherActor)
{
	Super::OnBeginHoisting_Implementation(OtherActor);
	Mesh->SetSimulatePhysics(false);
}

void AVehicleComponentActor::OnEndHoisting_Implementation(AActor* OtherActor)
{
	Super::OnEndHoisting_Implementation(OtherActor);
	Mesh->SetSimulatePhysics(true);
}
