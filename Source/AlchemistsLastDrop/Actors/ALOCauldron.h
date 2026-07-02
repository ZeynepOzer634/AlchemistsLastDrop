#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ALOInteractableInterface.h"
#include "Core/ALOCoreTypes.h"
#include "ALOCauldron.generated.h"

class UStaticMeshComponent;

UCLASS()
class ALCHEMISTSLASTDROP_API AALOCauldron : public AActor, public IALOInteractableInterface
{
	GENERATED_BODY()

public:
	AALOCauldron();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cauldron State")
	bool bIsBrewing = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cauldron State")
	bool bHasPotionReady = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cauldron Settings")
	float BrewingDuration = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cauldron State")
	TMap<EIngredientType, int32> CurrentIngredients;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Cauldron Settings")
	TArray<FALORecipe> AlchemyRecipes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Cauldron State")
	EALOPotion CurrentCookingPotion = EALOPotion::None;


	FTimerHandle BrewingTimerHandle;

	
	void OnBrewingComplete();

	
	EALOPotion CheckRecipes() const;

public:
	
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractableName_Implementation() const override;
};
