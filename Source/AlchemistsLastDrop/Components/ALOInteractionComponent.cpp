#include "Components/ALOInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/ALOInteractableInterface.h"

UALOInteractionComponent::UALOInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

AActor* UALOInteractionComponent::FindClosestInteractable() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (!OwnerPawn || !GetWorld())
	{
		return nullptr;
	}

	const FVector Center = OwnerPawn->GetActorLocation();

	TArray<FOverlapResult> OverlapResults;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerPawn);

	const FCollisionShape SphereShape = FCollisionShape::MakeSphere(InteractionRadius);

	const bool bHasOverlap = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		Center,
		FQuat::Identity,
		ObjectQueryParams,
		SphereShape,
		QueryParams
	);

	if (bDrawDebug)
	{
		DrawDebugSphere(
			GetWorld(),
			Center,
			InteractionRadius,
			24,
			bHasOverlap ? FColor::Green : FColor::Red,
			false,
			1.0f
		);
	}

	AActor* ClosestActor = nullptr;
	float ClosestDistanceSquared = TNumericLimits<float>::Max();

	for (const FOverlapResult& Result : OverlapResults)
	{
		AActor* CandidateActor = Result.GetActor();

		if (!CandidateActor || CandidateActor == OwnerPawn)
		{
			continue;
		}

		if (!CandidateActor->GetClass()->ImplementsInterface(UALOInteractableInterface::StaticClass()))
		{
			continue;
		}

		if (!IALOInteractableInterface::Execute_CanInteract(CandidateActor, OwnerPawn))
		{
			continue;
		}

		const float DistanceSquared = FVector::DistSquared(
			OwnerPawn->GetActorLocation(),
			CandidateActor->GetActorLocation()
		);

		if (DistanceSquared < ClosestDistanceSquared)
		{
			ClosestDistanceSquared = DistanceSquared;
			ClosestActor = CandidateActor;
		}
	}

	return ClosestActor;
}

void UALOInteractionComponent::TryInteract()
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (!OwnerPawn)
	{
		return;
	}

	AActor* ClosestInteractable = FindClosestInteractable();

	if (!ClosestInteractable)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				1.5f,
				FColor::Red,
				TEXT("No interactable in range")
			);
		}

		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.5f,
			FColor::Yellow,
			FString::Printf(TEXT("Interacting with: %s"), *ClosestInteractable->GetName())
		);
	}

	IALOInteractableInterface::Execute_Interact(ClosestInteractable, OwnerPawn);
}

void UALOInteractionComponent::PrimaryInteract()
{
	TryInteract();
}

float UALOInteractionComponent::GetInteractionRadius() const
{
	return InteractionRadius;
}