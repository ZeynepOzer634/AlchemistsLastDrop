#include "Actors/ALODispenser.h"
#include "Components/StaticMeshComponent.h"
#include "AlchemistsLastDropCharacter.h"
#include "Components/ALOInventoryComponent.h"

AALODispenser::AALODispenser()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

void AALODispenser::BeginPlay()
{
	Super::BeginPlay();
}

void AALODispenser::FillDispenser(EALOPotion PotionType)
{
	if (PotionType == EALOPotion::None) return;

	StoredPotionType = PotionType;
	bIsTankFull = true;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Green,
			FString::Printf(TEXT("Dispenser tank filled with: %s"), *UEnum::GetValueAsString(PotionType)));
	}
}

void AALODispenser::Interact_Implementation(AActor* Interactor)
{
	if (!Interactor) return;

	AAlchemistsLastDropCharacter* PlayerChar = Cast<AAlchemistsLastDropCharacter>(Interactor);
	if (!PlayerChar) return;

	//  Musluk tankı boşsa, oyuncunun elindeki sıvıyı depoya boşaltır
	if (!bIsTankFull)
	{
		EALOPotion HeldPotion = PlayerChar->GetCurrentHeldPotion();
		if (HeldPotion != EALOPotion::None)
		{
			
			FillDispenser(HeldPotion);
			PlayerChar->SetCurrentHeldPotion(EALOPotion::None);
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("You don't have any potion liquid in your hands!"));
			}
		}
	}
	//  Musluk tankı doluysa ve oyuncunun eli boşsa şişeleme yapar
	else
	{
		if (PlayerChar->GetCurrentHeldPotion() == EALOPotion::None)
		{
			
			PlayerChar->SetCurrentHeldPotion(StoredPotionType);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Orange,
					FString::Printf(TEXT("You bottled and collected: %s! Ready to serve villagers."), *UEnum::GetValueAsString(StoredPotionType)));
			}

			
			bIsTankFull = false;
			StoredPotionType = EALOPotion::None;
		}
		else
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Your hands are already full!"));
			}
		}
	}
}

FText AALODispenser::GetInteractableName_Implementation() const
{
	if (bIsTankFull) return FText::FromString(TEXT("Şişeye İksir Doldur"));
	return FText::FromString(TEXT("Kazandaki Sıvıyı Depoya Boşalt"));
}
