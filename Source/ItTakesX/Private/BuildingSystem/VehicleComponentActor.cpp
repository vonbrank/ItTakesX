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

	OnTakeAnyDamage.AddDynamic(this, &ThisClass::DamageTaken);
}

void AVehicleComponentActor::SetIsRunning(bool bNewIsRunning)
{
	bIsRunning = bNewIsRunning;
}

TScriptInterface<IVehicleNode> AVehicleComponentActor::GetVehicleRoot()
{
	if (ParentNode.GetInterface())
	{
		return ParentNode.GetInterface()->GetVehicleRoot();
	}
	TScriptInterface<IVehicleNode> VehicleRoot;
	VehicleRoot.SetInterface(this);
	VehicleRoot.SetObject(this);
	return VehicleRoot;
}

TArray<TScriptInterface<IVehicleNode>> AVehicleComponentActor::GetAllChildNodes()
{
	TArray<TScriptInterface<IVehicleNode>> AllChildNodes;

	for (auto ChildNode : ChildNodes)
	{
		auto ChildVehicleNode = ChildNode.GetInterface();

		if (ChildVehicleNode)
		{
			TArray<TScriptInterface<IVehicleNode>> ChildNodesArr = ChildVehicleNode->GetAllChildNodes();
			for (auto ChildChildNode : ChildNodesArr)
			{
				AllChildNodes.Add(ChildChildNode);
			}
		}
	}

	TScriptInterface<IVehicleNode> ThisNode;
	ThisNode.SetObject(this);
	ThisNode.SetInterface(this);
	AllChildNodes.Add(ThisNode);

	return AllChildNodes;
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

	if (
		VehicleNode != nullptr && // 另一个 Actor 必须是一个 IVehicleNode
		OtherComp // 另一个 Component 需要是连接点的 Comp
		->ComponentTags.Contains(TEXT("VehicleArea")))
	{
		AddNewNodeToVehicleNodeList(VehicleNode, OtherActor);

		// GEngine->AddOnScreenDebugMessage(
		// 	-1, 15.f, FColor::Yellow,
		// 	FString::Printf(
		// 		TEXT("Start overlap: %s, owner: %s, list length: %d"), *OtherComp->GetName(), *OtherActor->GetName(),
		// 		CurrentOverlappingVehicleNodes.Num()));
	}
}

void AVehicleComponentActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	auto VehicleNode = Cast<IVehicleNode>(OtherActor);
	if (VehicleNode != nullptr && OtherComp
	                              ->ComponentTags.Contains(TEXT("VehicleArea")))
	// 另一个 Component 需要是连接点的 Comp
	{
		RemoveNodeFromVehicleNodeList(VehicleNode);

		// GEngine->AddOnScreenDebugMessage(
		// 	-1, 15.f, FColor::Yellow,
		// 	FString::Printf(
		// 		TEXT("End overlap: %s, owner: %s, list length: %d"), *OtherComp->GetName(), *OtherActor->GetName(),
		// 		CurrentOverlappingVehicleNodes.Num()));
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

	// 关闭零件之间的碰撞
	TurnOnVehicleComponentCollisionChannel();
	CurrenNearestVehicleNode->TurnOnVehicleComponentCollisionChannel();


	ParentRootComponent->SetSimulatePhysics(false);
	Mesh->SetSimulatePhysics(false);


	ParentActor->AddActorWorldOffset(FVector::UpVector * 100);
	AddActorWorldOffset(FVector::UpVector * 100);
	ParentRootComponent->SetPhysicsLinearVelocity(FVector::Zero());
	Mesh->SetPhysicsLinearVelocity(FVector::Zero());

	// 修正旋转
	auto DirectionRotation = CurrentNearestOtherConnectionComponent->GetDirectionRotation(
		CurrentNearestConnectionComponent);
	SetActorRotation(DirectionRotation * GetActorRotation().Quaternion());
	// Mesh->SetPhysicsAngularVelocityInRadians(FVector::Zero());

	auto AlignmentRotation = CurrentNearestOtherConnectionComponent->GetAlignmentRotation(
		CurrentNearestConnectionComponent);
	SetActorRotation(AlignmentRotation * GetActorRotation().Quaternion());
	// Mesh->SetPhysicsAngularVelocityInRadians(FVector::Zero());


	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(TEXT("curren rotation: %s"), *GetActorRotation().ToString()));

	// 修正位置
	// SetActorLocation(CurrentNearestConnection.Arrow->GetComponentLocation());
	FVector DeltaLocation = CurrentNearestOtherConnectionComponent->GetDirectionArrowLocation()
		- CurrentNearestConnectionComponent->GetDirectionArrowLocation();
	AddActorWorldOffset(DeltaLocation);

	Mesh->SetPhysicsLinearVelocity(FVector::Zero());
	Mesh->SetPhysicsAngularVelocityInRadians(FVector::Zero());

	// FVector MidLocation = CurrentNearestConnectionComponent->GetComponentLocation() + (
	// 	CurrentNearestOtherConnectionComponent->
	// 	GetComponentLocation() - CurrentNearestConnectionComponent->GetComponentLocation()) / 2;

	// 生成物理约束
	auto PreviousConnectionConstraintActor = CurrentConnectionConstraintActor;
	APhysicsConstraintActor* PhysicsConstraintActor = GetWorld()->SpawnActor<APhysicsConstraintActor>();
	PhysicsConstraintActor->SetActorLocation(GetActorLocation());
	UPhysicsConstraintComponent* PhysicsConstraintComponent = PhysicsConstraintActor->GetConstraintComp();
	PhysicsConstraintComponent->SetDisableCollision(true);
	PhysicsConstraintComponent->SetConstrainedComponents(Mesh, TEXT(""), ParentRootComponent, TEXT(""));
	PhysicsConstraintComponent->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	PhysicsConstraintComponent->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	PhysicsConstraintComponent->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
	CurrentConnectionConstraintActor = PhysicsConstraintActor;

	Mesh->SetPhysicsLinearVelocity(FVector::Zero());
	Mesh->SetPhysicsAngularVelocityInRadians(FVector::Zero());

	//
	// ParentRootComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	// Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ParentRootComponent->SetSimulatePhysics(true);
	Mesh->SetSimulatePhysics(true);

	if (ParentNode.GetInterface())
	{
		TScriptInterface<IVehicleNode> ThisNode;
		ThisNode.SetInterface(this);
		ThisNode.SetObject(this);
		ParentNode.GetInterface()->ReverseAsRootNode(ThisNode, CurrentSelfConnection, CurrentOtherConnection,
		                                             PreviousConnectionConstraintActor);
	}

	ParentNode = CurrenNearestVehicleNode;
	if (ParentNode.GetInterface())
	{
		TScriptInterface<IVehicleNode> SonNode;
		SonNode.SetObject(this);
		SonNode.SetInterface(this);
		ParentNode->AddChildNode(SonNode);

		// 将连接点设置为连接状态
		CurrentSelfConnection = CurrentNearestConnectionComponent;
		CurrentOtherConnection = CurrentNearestOtherConnectionComponent;
		CurrentSelfConnection->SetConnectingState(true);
		CurrentOtherConnection->SetConnectingState(true);

		auto CurrentRoot = ParentNode->GetVehicleRoot();
		if (CurrentRoot)
		{
			auto TreeNodes = CurrentRoot->GetAllChildNodes();

			for (int i = 0; i < TreeNodes.Num(); i++)
			{
				auto NodeActor = Cast<AActor>(TreeNodes[i].GetInterface());
				if (NodeActor)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
					                                 FString::Printf(
						                                 TEXT("NodeActor %d: %s"), i, *NodeActor->GetName()));
				}
			}
		}
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

		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(TEXT("Same Connection, effect: %p"), CurrentAdsorbEffect));
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
		// 并查集
		if (GetVehicleRoot() == VehicleNode->GetVehicleRoot())
		{
			continue;
		}

		for (auto OtherVehicleConnectionComponent : VehicleNode->GetConnectionInfoList())
		{
			if (OtherVehicleConnectionComponent->IsConnecting())
			{
				continue;
			}
			for (auto VehicleConnectionComponent : VehicleConnectionComponentList)
			{
				if (VehicleConnectionComponent->IsConnecting())
				{
					continue;
				}
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

UStaticMeshComponent* AVehicleComponentActor::GetRootMesh()
{
	return Mesh;
}

bool AVehicleComponentActor::DetachFromParentVehicleNode()
{
	if (ParentNode.GetInterface() == nullptr)
	{
		return false;
	}

	if (CurrentConnectionConstraintActor == nullptr)
	{
		return false;
	}

	UPhysicsConstraintComponent* PhysicsConstraintComponent = CurrentConnectionConstraintActor->GetConstraintComp();
	PhysicsConstraintComponent->BreakConstraint();
	CurrentConnectionConstraintActor->Destroy();
	CurrentConnectionConstraintActor = nullptr;

	Mesh->AddForce(FVector::UpVector);

	if (ParentNode.GetInterface())
	{
		if (ParentNode.GetInterface()->GetRootMesh())
		{
			ParentNode.GetInterface()->GetRootMesh()->AddForce(FVector::UpVector);
		}
		TScriptInterface<IVehicleNode> VehicleNodeInterface;
		VehicleNodeInterface.SetInterface(this);
		VehicleNodeInterface.SetObject(this);
		ParentNode.GetInterface()->RemoveChildNode(VehicleNodeInterface);
		// 尝试重新启用父节点与零件间的碰撞
		ParentNode.GetInterface()->TryTurnOffVehicleComponentCollisionChannel();

		// 将连接点的连接状态取消
		CurrentSelfConnection->SetConnectingState(false);
		CurrentOtherConnection->SetConnectingState(false);
		CurrentSelfConnection = nullptr;
		CurrentOtherConnection = nullptr;
	}

	ParentNode.SetInterface(nullptr);
	ParentNode.SetObject(nullptr);

	// 重新启用与零件间的碰撞
	TryTurnOffVehicleComponentCollisionChannel();

	return true;
}

bool AVehicleComponentActor::DetachFromAdjacentChildVehicleNode()
{
	auto CurrenChildNodes = ChildNodes;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(TEXT("CurrenChildNodes num = %d"), CurrenChildNodes.Num()));
	for (auto ChildNodeInterface : CurrenChildNodes)
	{
		auto ChildNode = ChildNodeInterface.GetInterface();
		if (ChildNode)
		{
			ChildNode->DetachFromParentVehicleNode();
		}
	}

	return true;
}

bool AVehicleComponentActor::DetachFromAllAdjacentVehicleNode()
{
	bool bParentResult = DetachFromParentVehicleNode();
	bool bChildResult = DetachFromAdjacentChildVehicleNode();
	return bParentResult && bChildResult;
}

void AVehicleComponentActor::TurnOnVehicleComponentCollisionChannel()
{
	if (bNeedBuoyancy)
	{
		return;
	}
	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

bool AVehicleComponentActor::TryTurnOffVehicleComponentCollisionChannel()
{
	if (bNeedBuoyancy)
	{
		return true;
	}
	if (ParentNode.GetInterface() || ChildNodes.Num() > 0)
	{
		return false;
	}

	Mesh->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	Mesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
	return true;
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

bool AVehicleComponentActor::RemoveChildNode(TScriptInterface<IVehicleNode> VehicleNode)
{
	int RemoveIndex = -1;
	for (int i = 0; i < ChildNodes.Num(); i++)
	{
		if (ChildNodes[i].GetInterface() == VehicleNode.GetInterface())
		{
			RemoveIndex = i;
			break;
		}
	}
	if (RemoveIndex != -1)
	{
		ChildNodes.RemoveAt(RemoveIndex);
		return true;
	}
	return false;
}

void AVehicleComponentActor::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                         AController* DamageInstigator, AActor* DamageCauser)
{
	if (Cast<IVehicleNode>(DamageCauser) || DamageInstigator == GetWorld()->GetFirstPlayerController())
	{
		return;
	}

	auto VehicleComponentController = Cast<AVehicleComponentActor>(CurrentRunningVehicleController.GetInterface());
	if (VehicleComponentController)
	{
		VehicleComponentController->DamageTaken(DamagedActor, Damage, DamageType, DamageInstigator, DamageCauser);
	}
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

void AVehicleComponentActor::SetCurrentRunningRoot(TScriptInterface<IVehicleNode> NewVehicleRootNode)
{
	CurrentRunningRoot = NewVehicleRootNode;
}

TScriptInterface<IVehicleNode> AVehicleComponentActor::GetCurrentRunningRoot()
{
	return CurrentRunningRoot;
}

void AVehicleComponentActor::SetCurrentRunningVehicleController(
	TScriptInterface<IVehicleNode> NewVehicleControllerNode)
{
	CurrentRunningVehicleController = NewVehicleControllerNode;
}

TScriptInterface<IVehicleNode> AVehicleComponentActor::GetCurrentRunningVehicleController()
{
	return CurrentRunningVehicleController;
}

void AVehicleComponentActor::ReverseAsRootNode(TScriptInterface<IVehicleNode> NewParentNode,
                                               UVehicleConnectionComponent* NewCurrentSelfConnection,
                                               UVehicleConnectionComponent* NewCurrentOtherConnection,
                                               APhysicsConstraintActor* NewPhysicsConstraintActor)
{
	TScriptInterface<IVehicleNode> ThisNode;
	ThisNode.SetInterface(this);
	ThisNode.SetObject(this);

	if (ParentNode.GetInterface())
	{
		ParentNode.GetInterface()->ReverseAsRootNode(ThisNode, CurrentSelfConnection, CurrentOtherConnection,
		                                             CurrentConnectionConstraintActor);
	}

	int ChildIndex = -1;

	for (int i = 0; i < ChildNodes.Num(); i++)
	{
		auto ChildNodeInterface = ChildNodes[i];
		if (ChildNodeInterface.GetInterface() == NewParentNode.GetInterface())
		{
			ChildIndex = i;
			break;
		}
	}

	if (ChildIndex != -1)
	{
		ChildNodes.RemoveAt(ChildIndex);
	}

	CurrentSelfConnection = NewCurrentSelfConnection;
	CurrentOtherConnection = NewCurrentOtherConnection;
	CurrentConnectionConstraintActor = NewPhysicsConstraintActor;


	NewParentNode->AddChildNode(ThisNode);
	ParentNode = NewParentNode;
}
