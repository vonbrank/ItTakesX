// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ChariotPawn.h"

#include "BuildingSystem/Component/VehicleComponentBlade.h"
#include "Character/ItTakesXCharacter.h"
#include "Kismet/GameplayStatics.h"

AChariotPawn::AChariotPawn()
{
	for (int i = 1; i <= 6; i++)
	{
		UStaticMeshComponent* WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(
			*FString::Printf(TEXT("WeaponMesh%d"), i));
		WeaponMesh->SetupAttachment(RootComponent);
		WeaponMeshList.Add(WeaponMesh);
	}

	bHasDestroy = false;
}

void AChariotPawn::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < 6; i++)
	{
		WeaponMeshList[i]->OnComponentHit.AddDynamic(this, &ThisClass::AChariotPawn::OnWeaponHit);
	}
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("fuck")));
	// BodyMesh->OnComponentHit.AddDynamic(this, &ThisClass::AChariotPawn::OnWeaponHit);
}

void AChariotPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AChariotPawn::OnWeaponHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                               FVector NormalImpulse, const FHitResult& Hit)
{
	auto CurrentOwner = GetOwner();
	if (CurrentOwner == nullptr) return;

	auto OwnerInstigator = CurrentOwner->GetInstigatorController();
	auto DamageTypeClass = UDamageType::StaticClass();

	if (Cast<AItTakesXCharacter>(OtherActor))
	{
		UGameplayStatics::ApplyDamage(OtherActor, WeaponDamage, OwnerInstigator, this, DamageTypeClass);
	}
}

void AChariotPawn::LookAtTarget(FVector TargetPosition)
{
	Super::LookAtTarget(TargetPosition);

	if (bHasDestroy)
	{
		return;
	}

	// return;


	FVector VectorToTarget = TargetPosition - BodyMesh->GetComponentLocation();

	FVector NormalVector = BodyMesh->GetUpVector();

	FVector TargetForwardVector = VectorToTarget +
		NormalVector * (FVector::DotProduct(VectorToTarget, NormalVector) * -1);
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("ForwardVector = %s"), *BodyMesh->GetForwardVector().ToString()));
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
	//                                  FString::Printf(
	// 	                                 TEXT("TargetForwardVector = %s"), *TargetForwardVector.ToString()));
	TargetForwardVector.Normalize();
	float DiffAngle = FMath::RadiansToDegrees(
		FMath::Acos(FVector::DotProduct(TargetForwardVector, BodyMesh->GetForwardVector())));

	if (FVector::DotProduct(FVector::UpVector,
	                        FVector::CrossProduct(TargetForwardVector, BodyMesh->GetForwardVector())) > 0)
	{
		BodyMesh->AddTorqueInRadians(FVector::UpVector * -50, NAME_None, true);
	}
	else
	{
		BodyMesh->AddTorqueInRadians(FVector::UpVector * 50, NAME_None, true);
	}
}

void AChariotPawn::DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                               AController* DamageInstigator, AActor* DamageCauser)
{
	Super::DamageTaken(DamagedActor, Damage, DamageType, DamageInstigator, DamageCauser);

	if (Cast<AVehicleComponentBlade>(DamageCauser))
	{
		BreakPhysicsConstraintEvent();
		bHasDestroy = true;
	}
}
