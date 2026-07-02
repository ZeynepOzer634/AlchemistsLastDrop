#include "Components/ALOInventoryComponent.h"

UALOInventoryComponent::UALOInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false; 
}

void UALOInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UALOInventoryComponent::AddIngredient(EIngredientType Type, int32 Amount)
{
	if (Type == EIngredientType::None || Amount <= 0) return;

	int32& CurrentCount = IngredientStorage.FindOrAdd(Type, 0);
	CurrentCount += Amount;

	OnInventoryChanged.Broadcast(Type, CurrentCount);
}

bool UALOInventoryComponent::RemoveIngredient(EIngredientType Type, int32 Amount)
{
	if (Type == EIngredientType::None || Amount <= 0) return false;

	if (IngredientStorage.Contains(Type) && IngredientStorage[Type] >= Amount)
	{
		IngredientStorage[Type] -= Amount;
		
		if (IngredientStorage[Type] <= 0)
		{
			IngredientStorage.Remove(Type);
		}

		int32 NewCount = IngredientStorage.Contains(Type) ? IngredientStorage[Type] : 0;
		OnInventoryChanged.Broadcast(Type, NewCount);
		return true;
	}

	return false; 
}

int32 UALOInventoryComponent::GetIngredientCount(EIngredientType Type) const
{
	if (IngredientStorage.Contains(Type))
	{
		return IngredientStorage[Type];
	}
	return 0;
}