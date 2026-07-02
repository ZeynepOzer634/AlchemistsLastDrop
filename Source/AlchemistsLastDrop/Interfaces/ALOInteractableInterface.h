#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ALOInteractableInterface.generated.h"

UINTERFACE(Blueprintable)
class ALCHEMISTSLASTDROP_API UALOInteractableInterface : public UInterface
{
	GENERATED_BODY()
};

class ALCHEMISTSLASTDROP_API IALOInteractableInterface
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALO|Interaction")
	bool CanInteract(APawn* InteractingPawn) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALO|Interaction")
	void Interact(APawn* InteractingPawn);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ALO|Interaction")
	FText GetInteractionText() const;
};