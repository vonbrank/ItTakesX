// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/ChariotPawn.h"

#include "BuildingSystem/Component/VehicleComponentBlade.h"
#include "BuildingSystem/Component/VehicleComponentFlameThrower.h"
#include "BuildingSystem/Component/VehicleComponentTurret.h"
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

	if (bHasDestruct)
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

void AChariotPawn::Destruct(AActor* DestructCauser, AController* DestructInstigator)
{
	Super::Destruct(DestructCauser, DestructInstigator);

	if (Cast<AVehicleComponentBlade>(DestructCauser))
	{
		BreakPhysicsConstraintEvent();
		return;
	}

	if (Cast<AVehicleComponentFlameThrower>(DestructCauser))
	{
		BodyMesh->AddImpulse(FVector::UpVector * 500, NAME_None, true);
		BreakPhysicsConstraintEvent();
		return;
	}

	if (Cast<APlayerController>(DestructInstigator))
	{
		BodyMesh->AddImpulse(FVector::UpVector * 800, NAME_None, true);
		BreakPhysicsConstraintEvent();
	}
}
