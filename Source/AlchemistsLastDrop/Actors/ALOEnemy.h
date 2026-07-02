#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ALOEnemy.generated.h"

class UALOHealthComponent;

UCLASS()
class ALCHEMISTSLASTDROP_API AALOEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	AALOEnemy();

protected:
	virtual void BeginPlay() override;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UALOHealthComponent* HealthComponent;

	
	FTimerHandle AIChaseTimerHandle;

	
	void ChasePlayer();

	
	UFUNCTION()
	void HandleDeath(AActor* DeadActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI Settings")
	float AttackDamage = 10.f;

public:
	virtual void Tick(float DeltaTime) override;

	
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};