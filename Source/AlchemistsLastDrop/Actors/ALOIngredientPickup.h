#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ALOInteractableInterface.h"
#include "Core/ALOCoreTypes.h"
#include "ALOIngredientPickup.generated.h"

class UStaticMeshComponent;

UCLASS()
class ALCHEMISTSLASTDROP_API AALOIngredientPickup : public AActor, public IALOInteractableInterface
{
	GENERATED_BODY()
	
public:	
	AALOIngredientPickup();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Data")
	EIngredientType IngredientType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pickup Data")
	int32 PickupAmount = 1;

public:
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractableName_Implementation() const override;
};