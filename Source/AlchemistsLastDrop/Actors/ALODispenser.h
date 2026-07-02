#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ALOInteractableInterface.h"
#include "Core/ALOCoreTypes.h"
#include "ALODispenser.generated.h"

class UStaticMeshComponent;

UCLASS()
class ALCHEMISTSLASTDROP_API AALODispenser : public AActor, public IALOInteractableInterface
{
	GENERATED_BODY()

public:
	AALODispenser();

protected:
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dispenser State")
	EALOPotion StoredPotionType = EALOPotion::None;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dispenser State")
	bool bIsTankFull = false;

public:
	
	UFUNCTION(BlueprintCallable, Category = "Dispenser Actions")
	void FillDispenser(EALOPotion PotionType);

	
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractableName_Implementation() const override;
};
