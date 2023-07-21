// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingSystem/Component/VehicleComponentHoveringWheel.h"

void AVehicleComponentHoveringWheel::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (bIsRunning)
	{
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("running")));

		TArray<FHitResult> HitResults;
		GetWorld()->LineTraceMultiByChannel(
			HitResults, GetActorLocation(),
			GetActorLocation() + FVector::DownVector *
			TraceLength,
			ECollisionChannel::ECC_WorldDynamic);

		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		//                                  FString::Printf(TEXT("HitResults num = %d"), HitResults.Num()));

		bool bTranceResult = false;
		FHitResult HitResult;
		float MinDistance = TraceLength + 10;
		for (auto Result : HitResults)
		{
			auto VehicleNode = Cast<IVehicleNode>(Result.GetActor());

			if (VehicleNode)
			{
				continue;
			}

			if (MinDistance > (Result.Location - GetActorLocation()).Length())
			{
				MinDistance = (Result.Location - GetActorLocation()).Length();
				HitResult = Result;
				bTranceResult = true;
			}
		}


		if (bTranceResult)
		{
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
			//                                  FString::Printf(
			// 	                                 TEXT("HitResult actor name = %s"), *HitResult.GetActor()->GetName()));

			float CurrenForce = FMath::Lerp(HoverForce, 0,
			                                (HitResult.Location - GetActorLocation()).Length() / TraceLength);
			DrawDebugLine(GetWorld(), GetActorLocation(), HitResult.Location, FColor::Red, false,
			              GetWorld()->DeltaTimeSeconds);
			if (Mesh)
			{
				Mesh->AddForceAtLocation(HitResult.ImpactNormal * CurrenForce, GetActorLocation());
				Mesh->SetLinearDamping(3);
				Mesh->SetAngularDamping(5);
			}
			DrawDebugLine(
				GetWorld(),
				GetActorLocation(),
				HitResult.Location,
				FColor(255, 0, 0),
				false, -1, 0,
				1
			);
			DrawDebugPoint(GetWorld(), HitResult.Location, 10, FColor::Red);
		}
	}
}
