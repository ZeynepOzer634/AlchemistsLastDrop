#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ALOFreezeTrap.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class ALCHEMISTSLASTDROP_API AALOFreezeTrap : public AActor
{
	GENERATED_BODY()

public:
	AALOFreezeTrap();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp;

	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Trap Settings")
	float FreezeDuration = 3.0f;

	FTimerHandle FreezeTimerHandle;

	void UnfreezeEnemy(class AALOEnemy* TargetEnemy, float OriginalSpeed);

public:
	UFUNCTION()
	void OnTrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};