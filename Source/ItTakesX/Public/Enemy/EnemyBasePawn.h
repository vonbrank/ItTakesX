// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "EnemyBasePawn.generated.h"

UCLASS()
class ITTAKESX_API AEnemyBasePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AEnemyBasePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AreaSphere;

	UPROPERTY()
	class AItTakesXCharacter* CurrentTargetCharacter;

	UFUNCTION()
	virtual void OnSphereStartOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
	UFUNCTION()
	virtual void OnSphereEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	virtual void LookAtTarget(FVector TargetPosition);

	UFUNCTION(BlueprintPure)
	virtual bool IsTargetInRange() const;

	UFUNCTION()
	virtual void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                         AController* DamageInstigator, AActor* DamageCauser);

	UFUNCTION()
	virtual void RadialDamageTaken(
		AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin,
		const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY()
	class AEmitter* CurrentBurningEmitter;

	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

	float Health;

	bool bHasDestruct = false;

	virtual void Destruct(AActor* DestructCauser, AController* DestructInstigator);

	UPROPERTY()
	class AItTakesXGameMode* ItTakesXGameMode;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void FallDownBurningIfLiving();
};
