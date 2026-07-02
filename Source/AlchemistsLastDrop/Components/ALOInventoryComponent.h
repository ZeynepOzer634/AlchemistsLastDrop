#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Core/ALOCoreTypes.h"
#include "ALOInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryChanged, EIngredientType, IngredientType, int32, NewCount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALCHEMISTSLASTDROP_API UALOInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UALOInventoryComponent();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TMap<EIngredientType, int32> IngredientStorage;

public:	
	UPROPERTY(BlueprintAssignable, Category = "Inventory|Events")
	FOnInventoryChanged OnInventoryChanged;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddIngredient(EIngredientType Type, int32 Amount);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveIngredient(EIngredientType Type, int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetIngredientCount(EIngredientType Type) const;
};