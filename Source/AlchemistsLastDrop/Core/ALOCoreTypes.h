#pragma once

#include "CoreMinimal.h"
#include "ALOCoreTypes.generated.h"

/**
 * Oyundaki ham malzemelerin listesi
 */
UENUM(BlueprintType)
enum class EIngredientType : uint8
{
	None            UMETA(DisplayName = "None"),
	MoonHerb        UMETA(DisplayName = "Moon Herb"),
	RedMushroom     UMETA(DisplayName = "Red Mushroom"),
	CrystalDust     UMETA(DisplayName = "Crystal Dust"),
	BatWing         UMETA(DisplayName = "Bat Wing")
};

/**
 * Kazandan çıkan veya Mixing Counter'da üretilen son iksirlerin listesi
 */
UENUM(BlueprintType)
enum class EALOPotion : uint8
{
	None            UMETA(DisplayName = "None"),
	RawPotion       UMETA(DisplayName = "Raw Potion"),     // Kazandan ilk çıkan ham sıvı
	HealingPotion   UMETA(DisplayName = "Healing Potion"), // Moon Herb + Red Mushroom
	FirePotion      UMETA(DisplayName = "Fire Potion"),    // Crystal Dust + Ember Root/Bat Wing
	SpeedPotion     UMETA(DisplayName = "Speed Potion")    // Bat Wing + Moon Herb
};

/**
 * İksir üretmek için gerekli olan tarif yapısı
 */
USTRUCT(BlueprintType)
struct FALORecipe
{
	GENERATED_BODY()

	// Üretilecek iksir türü
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alchemy Recipe")
	EALOPotion PotionType = EALOPotion::None;

	// Bu iksir için hangi malzemeden kaçar adet gerekiyor?
	// (EALOIngredient hatası EIngredientType olarak düzeltildi)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Alchemy Recipe")
	TMap<EIngredientType, int32> RequiredIngredients;

	// Varsayılan Constructor (Hata önlemek için)
	FALORecipe() {}

	// Kolayca C++ içinde tarif tanımlayabilmek için yardımcı Constructor
	FALORecipe(EALOPotion InPotion, const TMap<EIngredientType, int32>& InIngredients)
		: PotionType(InPotion), RequiredIngredients(InIngredients) {}
};