#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALOInteractionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ALCHEMISTSLASTDROP_API UALOInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UALOInteractionComponent();

	UFUNCTION(BlueprintCallable, Category = "ALO|Interaction")
	void TryInteract();

	UFUNCTION(BlueprintCallable, Category = "ALO|Interaction")
	void PrimaryInteract();

	UFUNCTION(BlueprintCallable, Category = "ALO|Interaction")
	AActor* FindClosestInteractable() const;

	UFUNCTION(BlueprintCallable, Category = "ALO|Interaction")
	float GetInteractionRadius() const;

private:

	UPROPERTY(EditAnywhere, Category = "ALO|Interaction")
	float InteractionRadius = 220.0f;

	UPROPERTY(EditAnywhere, Category = "ALO|Interaction")
	bool bDrawDebug = true;
};