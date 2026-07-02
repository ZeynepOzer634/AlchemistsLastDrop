#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/ALOInteractableInterface.h"
#include "ALOSpeedPotion.generated.h"

class UStaticMeshComponent;

UCLASS()
class ALCHEMISTSLASTDROP_API AALOSpeedPotion : public AActor, public IALOInteractableInterface
{
	GENERATED_BODY()

public:
	AALOSpeedPotion();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PowerUp Settings")
	float SpeedMultiplier = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PowerUp Settings")
	float Duration = 5.0f;

	FTimerHandle SpeedTimerHandle;

	void ResetSpeed(class AAlchemistsLastDropCharacter* TargetChar, float OriginalSpeed);

public:
	virtual void Interact_Implementation(AActor* Interactor) override;
	virtual FText GetInteractableName_Implementation() const override;
};
