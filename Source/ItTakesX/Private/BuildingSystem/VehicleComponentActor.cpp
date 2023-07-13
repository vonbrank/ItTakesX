// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/VehicleComponentActor.h"

#include "Components/ArrowComponent.h"
#include "Components/SphereComponent.h"
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

	CurrentConnectionIndex = -1;
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
		auto StaticMeshComp = Cast<UStaticMeshComponent>(Comp);
		if (Comp->ComponentTags.Contains(TEXT("ConnectionComp")) && StaticMeshComp)
		{
			ConnectionComponents.Add(StaticMeshComp);
		}
	}

	for (auto Comp : ConnectionComponents)
	{
		auto Material = Comp->CreateDynamicMaterialInstance(0);
		ConnectionMaterials.Add(Material);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(
		                                 TEXT("name: %s, connection num: %d"), *GetName(), ConnectionComponents.Num()));
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

	if (CurrentHoistingActor != nullptr && CurrentOverlappingComponent == nullptr && OtherComp->ComponentTags.Contains(
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
		if (VehicleNode)
		{
			VehicleNode->DeactivateAllConnection();
		}

		CurrentOverlappingVehicleNode.SetObject(nullptr);
		CurrentOverlappingVehicleNode.SetInterface(nullptr);
	}
}

void AVehicleComponentActor::AddChildNode(TScriptInterface<IVehicleNode> ChildNode)
{
}

bool AVehicleComponentActor::AttachToCurrentOverlappingVehicleNode()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(
		                                 TEXT("OutConnectionIndex: %d"), CurrentConnectionIndex));

	if (CurrentConnectionIndex == -1) return false;

	auto ParentActor = Cast<AActor>(CurrentOverlappingVehicleNode.GetInterface());

	if (ParentActor == nullptr) return false;
	auto ParentRootComponent = Cast<UPrimitiveComponent>(ParentActor->GetRootComponent());
	if (ParentRootComponent == nullptr)
	{
		return false;
	}

	// SetActorLocation(FMath::VInterpTo(GetActorLocation(), CurrentPlaceLocation,
	//                                   UGameplayStatics::GetWorldDeltaSeconds(this), 5.f));
	SetActorLocation(CurrentPlaceLocation);

	FVector MidLocation = ParentActor->GetActorLocation() + (CurrentPlaceLocation - CurrentPlaceLocation) / 2;

	APhysicsConstraintActor* PhysicsConstraintActor = GetWorld()->SpawnActor<APhysicsConstraintActor>();
	PhysicsConstraintActor->SetActorLocation(MidLocation);
	UPhysicsConstraintComponent* PhysicsConstraintComponent = PhysicsConstraintActor->GetConstraintComp();
	PhysicsConstraintComponent->SetConstrainedComponents(ParentRootComponent, TEXT(""), Mesh, TEXT(""));
	PhysicsConstraintComponent->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	PhysicsConstraintComponent->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	PhysicsConstraintComponent->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
	PhysicsConstraintComponent->SetDisableCollision(true);
	CurrentOverlappingVehicleNode.GetInterface()->DeactivateAllConnection();
	// Mesh->SetSimulatePhysics(true);
	return true;
}

void AVehicleComponentActor::AddConnectionComponent(USceneComponent* Component)
{
	auto StaticMesh = Cast<UStaticMeshComponent>(Component);
	ConnectionComponents.Add(StaticMesh);
}

bool AVehicleComponentActor::IsHoisting() const
{
	return CurrentHoistingActor != nullptr;
}

void AVehicleComponentActor::NearestConnection(FVector SourceLocation, int32& OutConnectionIndex,
                                               FVector& OutConnectionLocation,
                                               FVector& OutPlaceLocation) const
{
	OutConnectionIndex = -1;
	float MinDistance = TNumericLimits<float>::Max();

	for (auto i = 0; i < ConnectionComponents.Num(); i++)
	{
		auto ConnectionComponent = ConnectionComponents[i];
		UArrowComponent* ArrowComponent = Cast<UArrowComponent>(ConnectionComponent->GetChildComponent(0));
		if (ArrowComponent == nullptr)
		{
			continue;
		}
		auto ConnectionLocation = ConnectionComponent->GetComponentLocation();
		auto PlaceLocation = ArrowComponent->GetComponentLocation();
		auto CurrentDistance = (SourceLocation - ConnectionLocation).Length();
		if (CurrentDistance < MinDistance)
		{
			MinDistance = CurrentDistance;
			OutConnectionIndex = i;
			OutConnectionLocation = ConnectionLocation;
			OutPlaceLocation = PlaceLocation;
		}
	}
}

void AVehicleComponentActor::GetBothWayConnectionInfo(const TArray<FVector>& SourceAnchorLocations,
                                                      const TArray<FVector>& SourceArrowLocations,
                                                      const TArray<FRotator>& SourceArrowRotation,
                                                      int32& OutSourceConnectionIndex,
                                                      int32& OutTargetConnectionIndex,
                                                      FVector& OutConnectionLocation,
                                                      FVector& OutPlaceLocation)
{
	// IVehicleNode::GetBothWayConnectionInfo(SourceAnchorLocations, SourceArrowLocations, SourceArrowRotation,
	//                                        OutSourceConnectionIndex,
	//                                        OutTargetConnectionIndex, OutConnectionLocation, OutPlaceLocation);

	// OutSourceConnectionIndex = -1;
	// OutTargetConnectionIndex = -1;
	// float MinDistance = TNumericLimits<float>::Max();
	//
	// if(SourceAnchorLocations.Num() != SourceArrowLocations.Num())
	//
	// for (auto i = 0; i < ConnectionComponents.Num(); i++)
	// {
	// 	auto ConnectionComponent = ConnectionComponents[i];
	// 	UArrowComponent* ArrowComponent = Cast<UArrowComponent>(ConnectionComponent->GetChildComponent(0));
	// 	if (ArrowComponent == nullptr)
	// 	{
	// 		continue;
	// 	}
	//
	// 	for(auto j = 0; )
	// 	
	// 	
	// 	auto ConnectionLocation = ConnectionComponent->GetComponentLocation();
	// 	auto PlaceLocation = ArrowComponent->GetComponentLocation();
	// 	auto CurrentDistance = (SourceLocation - ConnectionLocation).Length();
	// 	if (CurrentDistance < MinDistance)
	// 	{
	// 		MinDistance = CurrentDistance;
	// 		OutConnectionIndex = i;
	// 		OutConnectionLocation = ConnectionLocation;
	// 		OutPlaceLocation = PlaceLocation;
	// 	}
	// }
}

void AVehicleComponentActor::ActivateConnection(int32 ConnectionIndex)
{
	for (int32 i = 0; i < ConnectionMaterials.Num(); i++)
	{
		auto Material = ConnectionMaterials[i];
		if (i == ConnectionIndex)
		{
			Material->SetScalarParameterValue(TEXT("Transparent"), 1.f);
		}
		else
		{
			Material->SetScalarParameterValue(TEXT("Transparent"), 0.f);
		}
	}
}

bool AVehicleComponentActor::InteractWithOverlappingVehicleNode()
{
	auto VehicleNode = CurrentOverlappingVehicleNode.GetInterface();

	//
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("VehicleNode: %p, CurrentHoistingActor: %p"), VehicleNode,
	// 	                                 CurrentHoistingActor));

	if (CurrentHoistingActor == nullptr || VehicleNode == nullptr)
	{
		return false;
	}

	FVector SourceConnection = GetActorLocation();
	int32 OutConnectionIndex = -1;
	FVector OutConnectionLocation;
	FVector OutPlaceLocation;


	VehicleNode->NearestConnection(SourceConnection, OutConnectionIndex, OutConnectionLocation, OutPlaceLocation);
	VehicleNode->ActivateConnection(OutConnectionIndex);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(
		                                 TEXT("OutConnectionIndex: %d"), OutConnectionIndex));

	CurrentConnectionIndex = OutConnectionIndex;

	if (OutConnectionIndex >= 0)
	{
		CurrentPlaceLocation = OutPlaceLocation;
	}

	return true;
}

void AVehicleComponentActor::DeactivateAllConnection()
{
	// for (int32 i = 0; i < ConnectionMaterials.Num(); i++)
	// {
	// 	auto Material = ConnectionMaterials[i];
	// 	Material->SetScalarParameterValue(TEXT("Transparent"), 0.f);
	// }
	// for (int32 i = 0; i < ConnectionComponents.Num(); i++)
	// {
	// 	
	// }
}
