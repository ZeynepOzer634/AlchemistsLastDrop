#include "Components/ALOHealthComponent.h"
#include "GameFramework/Actor.h"

UALOHealthComponent::UALOHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UALOHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	// Aktör hasar aldığında tetiklenecek fonksiyonu Unreal'ın delegatına bağlıyoruz
	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeAnyDamage.AddDynamic(this, &UALOHealthComponent::HandleTakeAnyDamage);
	}
}

void UALOHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f || CurrentHealth <= 0.f) return;

	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red,
			FString::Printf(TEXT("%s took %f damage! Remaining Health: %f"), *GetOwner()->GetName(), Damage, CurrentHealth));
	}

	if (CurrentHealth <= 0.f)
	{
		OnDeath.Broadcast(GetOwner());
	}
}