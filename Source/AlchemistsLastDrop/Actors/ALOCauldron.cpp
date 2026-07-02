#include "Actors/ALOCauldron.h"
#include "Components/StaticMeshComponent.h"
#include "AlchemistsLastDropCharacter.h"
#include "Components/ALOInventoryComponent.h"
#include "TimerManager.h"

AALOCauldron::AALOCauldron()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

void AALOCauldron::BeginPlay()
{
	Super::BeginPlay();

	// 1. Healing Potion = 1 Moon Herb + 1 Red Mushroom
	TMap<EIngredientType, int32> HealIngredients;
	HealIngredients.Add(EIngredientType::MoonHerb, 1);
	HealIngredients.Add(EIngredientType::RedMushroom, 1);
	AlchemyRecipes.Add(FALORecipe(EALOPotion::HealingPotion, HealIngredients));

	// 2. Fire Potion = 1 Crystal Dust + 1 Bat Wing
	TMap<EIngredientType, int32> FireIngredients;
	FireIngredients.Add(EIngredientType::CrystalDust, 1);
	FireIngredients.Add(EIngredientType::BatWing, 1);
	AlchemyRecipes.Add(FALORecipe(EALOPotion::FirePotion, FireIngredients));
}

void AALOCauldron::Interact_Implementation(AActor* Interactor)
{
	if (!Interactor || bIsBrewing) return;

	AAlchemistsLastDropCharacter* PlayerChar = Cast<AAlchemistsLastDropCharacter>(Interactor);
	if (!PlayerChar) return;

	UALOInventoryComponent* Inventory = PlayerChar->GetInventoryComponent();
	if (!Inventory) return;

	//  Kazanda pişen iksir hazırsa ve oyuncunun eli boşsa oyuncu bunu teslim alır
	if (bHasPotionReady)
	{
		if (PlayerChar->GetCurrentHeldPotion() == EALOPotion::None)
		{
			// İksiri karakterin eline teslim et
			PlayerChar->SetCurrentHeldPotion(CurrentCookingPotion);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan,
					FString::Printf(TEXT("You collected: %s into your hands! Carry it to the Dispenser."), *UEnum::GetValueAsString(CurrentCookingPotion)));
			}

			// Kazanı sıfırla
			bHasPotionReady = false;
			CurrentCookingPotion = EALOPotion::None;
			CurrentIngredients.Empty();
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Your hands are full! Empty them first."));
			}
		}
		return;
	}

	//  Kazan boş, oyuncu elindeki malzemeleri kazana atıyor 
	if (PlayerChar->GetCurrentHeldPotion() != EALOPotion::None) return;

	if (Inventory->GetIngredientCount(EIngredientType::MoonHerb) > 0)
	{
		Inventory->RemoveIngredient(EIngredientType::MoonHerb, 1);
		CurrentIngredients.FindOrAdd(EIngredientType::MoonHerb, 0)++;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Added 1 Moon Herb to Cauldron!"));
		}
	}
	else if (Inventory->GetIngredientCount(EIngredientType::RedMushroom) > 0)
	{
		Inventory->RemoveIngredient(EIngredientType::RedMushroom, 1);
		CurrentIngredients.FindOrAdd(EIngredientType::RedMushroom, 0)++;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Added 1 Red Mushroom to Cauldron!"));
		}
	}

	EALOPotion MatchedPotion = CheckRecipes();
	if (MatchedPotion != EALOPotion::None)
	{
		bIsBrewing = true;
		CurrentCookingPotion = MatchedPotion;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Yellow, TEXT("Recipe matched! Brewing started..."));
		}

		GetWorld()->GetTimerManager().SetTimer(BrewingTimerHandle, this, &AALOCauldron::OnBrewingComplete, BrewingDuration, false);
	}
}

EALOPotion AALOCauldron::CheckRecipes() const
{
	for (const FALORecipe& Recipe : AlchemyRecipes)
	{
		bool bMatchFailed = false;

		for (const auto& RequiredIngredient : Recipe.RequiredIngredients)
		{
			EIngredientType ReqType = RequiredIngredient.Key;
			int32 ReqAmount = RequiredIngredient.Value;

			if (!CurrentIngredients.Contains(ReqType) || CurrentIngredients[ReqType] < ReqAmount)
			{
				bMatchFailed = true;
				break;
			}
		}

		if (!bMatchFailed)
		{
			return Recipe.PotionType;
		}
	}

	return EALOPotion::None;
}

void AALOCauldron::OnBrewingComplete()
{
	bIsBrewing = false;
	bHasPotionReady = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("Cauldron: Brewing Complete! Potion is ready to collect!"));
	}
}

FText AALOCauldron::GetInteractableName_Implementation() const
{
	if (bHasPotionReady) return FText::FromString(TEXT("İksiri Kazandan Al"));
	if (bIsBrewing) return FText::FromString(TEXT("İksir Pişiyor..."));
	return FText::FromString(TEXT("Malzeme Ekle"));
}
