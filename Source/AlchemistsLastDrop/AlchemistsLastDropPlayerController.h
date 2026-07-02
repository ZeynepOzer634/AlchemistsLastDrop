#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AlchemistsLastDropPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class ALCHEMISTSLASTDROP_API AAlchemistsLastDropPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAlchemistsLastDropPlayerController();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	int32 MappingContextPriority = 0;
};