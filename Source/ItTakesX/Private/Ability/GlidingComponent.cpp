// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/GlidingComponent.h"

#include "Ability/InventoryComponent.h"
#include "Character/ItTakesXCharacter.h"
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

	Character = Cast<AItTakesXCharacter>(GetOwner());
	InventoryComponent = GetOwner()->FindComponentByClass<UInventoryComponent>();
}


// Called every frame
void UGlidingComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (Character)
	{
		if (bIsGliding && Character->GetCharacterMovement()->IsMovingOnGround())
		{
			StopGliding();
		}
		else if (bIsGliding)
		{
			float CharacterMass = Character->GetCharacterMovement()->Mass;

			Character->GetCharacterMovement()->AddForce(
				FVector::UpVector * CharacterMass * GlidingAirDrag * -Character->GetCharacterMovement()->Velocity.Z);
			if (Character->GetCharacterMovement()->Velocity.Z <= 0)
			{
				// FVector NewVelocity = Character->GetMovementComponent()->Velocity;
				// if (NewVelocity.Z < -MaxGlidingFallingSpeed)
				// {
				// 	NewVelocity.Z = -MaxGlidingFallingSpeed;
				// 	Character->GetMovementComponent()->Velocity = NewVelocity;
				// }
			}
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			//                                  FString::Printf(
			// 	                                 TEXT("Character velocity: %s"),
			// 	                                 *Character->GetCharacterMovement()->Velocity.ToString()));
		}
	}
}

bool UGlidingComponent::StartGliding()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Start Gliding")));

	if (Character == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Character is null")));
		return false;
	}

	if (Character->GetCharacterMovement()->IsMovingOnGround())
	{
		float CharacterMass = Character->GetCharacterMovement()->Mass;
		Character->GetCharacterMovement()->AddImpulse(FVector::UpVector * CharacterMass * CharacterLaunchSpeed * 1000);

		return false;
	}

	if (Character->GetCharacterMovement()->IsFalling())
	{
		// Character->GetCharacterMovement()->GravityScale = 0.15;
		bIsGliding = true;
		Character->GetCharacterMovement()->AirControl = 0.7;
		// auto NewVelocity = Character->GetCharacterMovement()->Velocity;
		// NewVelocity.Z = 0;
		// Character->GetCharacterMovement()->Velocity = NewVelocity;

		InventoryComponent->SwitchToEquippableByIndex(4);

		// if (CurrenGlider)
		// {
		// 	CurrenGlider->Destroy();
		// }
		// CurrenGlider = GetWorld()->SpawnActor<AGlider>(GliderClass);
		//
		// if (Character)
		// {
		// 	TScriptInterface<IEquippable> EquippableInterface;
		// 	EquippableInterface.SetObject(CurrenGlider);
		// 	EquippableInterface.SetInterface(CurrenGlider);
		//
		// 	Character->PickUpAndEquip(EquippableInterface);
		// }
	}
	else
	{
		return false;
	}


	return true;
}

bool UGlidingComponent::StopGliding()
{
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("start stop gliding")));

	if (Character == nullptr)
	{
		return false;
	}

	bIsGliding = false;
	// Character->GetCharacterMovement()->GravityScale = 1.0;
	Character->GetCharacterMovement()->AirControl = 0.2;

	// TScriptInterface<IEquippable> EquippableInterface;
	// Character->PickUpAndEquip(EquippableInterface);

	if (InventoryComponent)
	{
		if (Cast<AGlider>(InventoryComponent->GetCurrentEquippable()))
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			//                                  FString::Printf(TEXT("toggle after glading un equip")));
			InventoryComponent->UnEquipCurrenEquippableWithBroadcast();
		}
	}

	// if (CurrenGlider)
	// {
	// 	CurrenGlider->Destroy();
	// 	CurrenGlider = nullptr;
	// }

	return true;
}

bool UGlidingComponent::IsGliding() const
{
	return bIsGliding;
}

bool UGlidingComponent::ToggleGliding()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("start toggle gliding")));

	if (bIsGliding)
	{
		return StopGliding();
	}
	return StartGliding();
}
