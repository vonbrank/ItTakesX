// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBasePawn.h"

#include "Character/ItTakesXCharacter.h"
#include "Components/SphereComponent.h"

// Sets default values
AEnemyBasePawn::AEnemyBasePawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("BodyMesh");
	SetRootComponent(BodyMesh);

	AreaSphere = CreateDefaultSubobject<USphereComponent>("AreaSphere");
	AreaSphere->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyBasePawn::BeginPlay()
{
	Super::BeginPlay();

	AreaSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereStartOverlap);
	AreaSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnSphereEndOverlap);

	OnTakeAnyDamage.AddDynamic(this, &ThisClass::AEnemyBasePawn::DamageTaken);
}

// Called every frame
void AEnemyBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsTargetInRange())
	{
		LookAtTarget(CurrentTargetCharacter->GetActorLocation());
	}
}

// Called to bind functionality to input
void AEnemyBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBasePawn::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                          UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                          const FHitResult& SweepResult)
{
	AItTakesXCharacter* NewCharacter = Cast<AItTakesXCharacter>(OtherActor);
	if (NewCharacter)
	{
		CurrentTargetCharacter = NewCharacter;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("New target character entered.")));
	}
}

void AEnemyBasePawn::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == CurrentTargetCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("target character left.")));
		CurrentTargetCharacter = nullptr;
	}
}

void AEnemyBasePawn::LookAtTarget(FVector TargetPosition)
{
}

bool AEnemyBasePawn::IsTargetInRange() const
{
	return CurrentTargetCharacter != nullptr;
}

void AEnemyBasePawn::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                 AController* DamageInstigator, AActor* DamageCauser)
{
}
