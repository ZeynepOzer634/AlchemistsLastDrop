#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AlchemistsLastDropGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameOver, bool, bIsVictory);

UCLASS()
class ALCHEMISTSLASTDROP_API AAlchemistsLastDropGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AAlchemistsLastDropGameMode();

protected:

	virtual void BeginPlay() override;

public:

	UPROPERTY(BlueprintAssignable, Category = "ALO|GameMode")
	FOnGameOver OnGameOver;

	UFUNCTION(BlueprintCallable, Category = "ALO|GameMode")
	void EndGame(bool bIsVictory);
};
