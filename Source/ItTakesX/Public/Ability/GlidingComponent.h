// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Glider.h"
#include "Components/ActorComponent.h"
#include "GlidingComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ITTAKESX_API UGlidingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGlidingComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool bIsGliding;

	bool StartGliding();
	bool StopGliding();

	UPROPERTY()
	class AItTakesXCharacter* Character;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGlider> GliderClass;

	UPROPERTY()
	AGlider* CurrenGlider;

protected:
public:
	bool ToggleGliding();
	bool IsGliding() const;
};
