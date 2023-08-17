// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTAKESX_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealthComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;
	float Health = 0.f;

	UFUNCTION()
	void DamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	                 AController* Instigator, AActor* DamageCauser);

	UPROPERTY()
	class UDrivingComponent* DrivingComponent;

	UPROPERTY()
	class AItTakesXGameMode* ItTakesXGameMode;

public:
	float GetHealthPercentage();

	FORCEINLINE float GetMaxHealth() { return MaxHealth; }

	FORCEINLINE float GetHealth() { return Health; }
	FORCEINLINE void ResetHealth() { Health = MaxHealth; }
};
