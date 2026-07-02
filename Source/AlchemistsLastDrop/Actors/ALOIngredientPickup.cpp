#include "Actors/ALOIngredientPickup.h"
#include "Components/StaticMeshComponent.h"
#include "AlchemistsLastDropCharacter.h" 
#include "Components/ALOInventoryComponent.h"

AALOIngredientPickup::AALOIngredientPickup()
{
	PrimaryActorTick.bCanEverTick = false; 

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	IngredientType = EIngredientType::MoonHerb;
}

void AALOIngredientPickup::BeginPlay()
{
	Super::BeginPlay();
}

void AALOIngredientPickup::Interact_Implementation(AActor* Interactor)
{
	if (!Interactor) return;

	AAlchemistsLastDropCharacter* PlayerChar = Cast<AAlchemistsLastDropCharacter>(Interactor);
	if (PlayerChar)
	{
		UALOInventoryComponent* Inventory = PlayerChar->GetInventoryComponent();
		if (Inventory)
		{
			Inventory->AddIngredient(IngredientType, PickupAmount);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, 
					FString::Printf(TEXT("Collected: %d units of Ingredient!"), PickupAmount));
			}

			Destroy();
		}
	}
}

FText AALOIngredientPickup::GetInteractableName_Implementation() const
{
	return FText::FromString(TEXT("Malzemeyi Topla"));
}