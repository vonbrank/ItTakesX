// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyBasePawn.h"

#include "BuildingSystem/Component/VehicleComponentBlade.h"
#include "BuildingSystem/Component/VehicleComponentFlameThrower.h"
#include "Character/ItTakesXCharacter.h"
#include "Components/SphereComponent.h"
#include "Projectile/BaseProjectile.h"

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

	OnTakeAnyDamage.AddDynamic(this, &ThisClass::DamageTaken);
	OnTakeRadialDamage.AddDynamic(this, &ThisClass::RadialDamageTaken);

	Health = MaxHealth;
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

void AEnemyBasePawn::FallDownBurningIfLiving()
{
	if (CurrentBurningEmitter && Health > 0.f)
	{
		CurrentBurningEmitter->Destroy();
		CurrentBurningEmitter = nullptr;
	}
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
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(TEXT(" is damaging enemy")));

	if (Cast<AVehicleComponentBlade>(DamageCauser))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(TEXT("%s is damaging by AVehicleComponentBlade"), *GetName()));
		return;
	}

	auto VehicleComponentFlameThrower = Cast<AVehicleComponentFlameThrower>(DamageCauser);

	if (VehicleComponentFlameThrower)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(
		// 	                                 TEXT("%s is damaging by AVehicleComponentFlameThrower"), *GetName()));
		if (CurrentBurningEmitter == nullptr)
		{
			CurrentBurningEmitter = GetWorld()->SpawnActor<AEmitter>(VehicleComponentFlameThrower->GetFireClass(),
			                                                         GetActorLocation(), GetActorRotation());
			if (CurrentBurningEmitter)
			{
				CurrentBurningEmitter->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
	}
	Health -= Damage;
}

void AEnemyBasePawn::RadialDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                       FVector Origin, const FHitResult& HitInfo, AController* InstigatedBy,
                                       AActor* DamageCauser)
{
	auto Projectile = Cast<ABaseProjectile>(DamageCauser);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	                                 FString::Printf(
		                                 TEXT("%s is damaging by AVehicleComponentFlameThrower"), *GetName()));

	if (Projectile)
	{
		if (CurrentBurningEmitter == nullptr)
		{
			CurrentBurningEmitter = GetWorld()->SpawnActor<AEmitter>(Projectile->GetFireClass(),
			                                                         GetActorLocation(), GetActorRotation());
			if (CurrentBurningEmitter)
			{
				CurrentBurningEmitter->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
		}
		Health -= Damage;
	}
}
