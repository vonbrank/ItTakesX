// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GlidingComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UGlidingComponent::UGlidingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGlidingComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ACharacter>(GetOwner());
}


// Called every frame
void UGlidingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (Character)
	{
		if (Character->GetCharacterMovement()->IsMovingOnGround())
		{
			StopGliding();
		}
	}
}

bool UGlidingComponent::StartGliding()
{
	if (Character == nullptr)
	{
		return false;
	}

	bIsGliding = true;
	if (Character->GetCharacterMovement()->IsFalling())
	{
		Character->GetCharacterMovement()->GravityScale = 0.07;
		Character->GetCharacterMovement()->AirControl = 0.7;
		auto NewVelocity = Character->GetCharacterMovement()->Velocity;
		NewVelocity.Z = 0;
		Character->GetCharacterMovement()->Velocity = NewVelocity;
	}
	else if (Character->GetCharacterMovement()->IsMovingOnGround())
	{
	}
	return true;
}

bool UGlidingComponent::StopGliding()
{
	if (Character == nullptr)
	{
		return false;
	}

	bIsGliding = false;
	Character->GetCharacterMovement()->GravityScale = 1.0;
	Character->GetCharacterMovement()->AirControl = 0.2;

	return true;
}

bool UGlidingComponent::IsGliding() const
{
	return bIsGliding;
}

bool UGlidingComponent::ToggleGliding()
{
	if (bIsGliding)
	{
		return StopGliding();
	}
	return StartGliding();
}
