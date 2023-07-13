// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/VehicleComponentActor.h"

#include "Components/SphereComponent.h"
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
}

// Called every frame
void AVehicleComponentActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

		CurrentOverlappingVehicleNode.SetObject(nullptr);
		CurrentOverlappingVehicleNode.SetInterface(nullptr);
	}
}

void AVehicleComponentActor::AddChildNode(TScriptInterface<IVehicleNode> ChildNode)
{
}

bool AVehicleComponentActor::AttachToCurrentOverlappingVehicleNode()
{
	auto ParentActor = Cast<AActor>(CurrentOverlappingVehicleNode.GetInterface());

	if (ParentActor == nullptr) return false;
	auto ParentRootComponent = Cast<UPrimitiveComponent>(ParentActor->GetRootComponent());
	if (ParentRootComponent == nullptr)
	{
		return false;
	}

	APhysicsConstraintActor* PhysicsConstraintActor = GetWorld()->SpawnActor<APhysicsConstraintActor>();
	UPhysicsConstraintComponent* PhysicsConstraintComponent = PhysicsConstraintActor->GetConstraintComp();
	PhysicsConstraintComponent->SetConstrainedComponents(ParentRootComponent, TEXT(""), Mesh, TEXT(""));
	PhysicsConstraintComponent->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	PhysicsConstraintComponent->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Locked, 0.f);
	PhysicsConstraintComponent->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.f);
	// Mesh->SetSimulatePhysics(true);
	return true;
}
