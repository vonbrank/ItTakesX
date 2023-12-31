// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/VehicleComponentBlade.h"

#include "Enemy/EnemyBasePawn.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/RadialForceActor.h"
#include "PhysicsEngine/RadialForceComponent.h"

AVehicleComponentBlade::AVehicleComponentBlade()
{
	RotatingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotatingMesh"));
	RotatingMesh->SetupAttachment(RootComponent);
}

void AVehicleComponentBlade::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bIsRunning)
	{
		auto NewRotation = RotatingMesh->GetRelativeRotation();
		NewRotation.Yaw += RotatingSpeed * DeltaSeconds;
		RotatingMesh->SetRelativeRotation(NewRotation);
	}
}

void AVehicleComponentBlade::BeginPlay()
{
	Super::BeginPlay();

	Mesh->OnComponentHit.AddDynamic(this, &ThisClass::OnWeaponHit);
}

void AVehicleComponentBlade::OnWeaponHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<AEnemyBasePawn>(OtherActor))
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("blade on hit")));
		auto DamageType = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(OtherActor, Damage, GetWorld()->GetFirstPlayerController(), this, DamageType);

		if (OtherActor->IsRootComponentMovable())
		{
			OtherComp->AddImpulse(GetActorForwardVector() * 500, NAME_None, true);
		}

		// auto Explosion = GetWorld()->SpawnActor<ARadialForceActor>();
		// if (Explosion)
		// {
		// 	Explosion->SetActorLocation(Hit.Location);
		// 	auto ForceComponent = Explosion->GetForceComponent();
		// 	ForceComponent->ImpulseStrength = 10000;
		// 	ForceComponent->ForceStrength = 10000;
		// }
	}
}
