#include "Actors/ALOSpeedPotion.h"
#include "Components/StaticMeshComponent.h"
#include "AlchemistsLastDropCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

AALOSpeedPotion::AALOSpeedPotion()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

void AALOSpeedPotion::BeginPlay()
{
	Super::BeginPlay();
}

void AALOSpeedPotion::Interact_Implementation(AActor* Interactor)
{
	if (!Interactor) return;

	AAlchemistsLastDropCharacter* PlayerChar = Cast<AAlchemistsLastDropCharacter>(Interactor);
	if (PlayerChar && PlayerChar->GetCharacterMovement())
	{
		float OriginalSpeed = PlayerChar->GetCharacterMovement()->MaxWalkSpeed;

		PlayerChar->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed * SpeedMultiplier;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Yellow, TEXT("SPEED POWER-UP ACTIVATED!"));
		}

		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &AALOSpeedPotion::ResetSpeed, PlayerChar, OriginalSpeed);
		GetWorld()->GetTimerManager().SetTimer(SpeedTimerHandle, TimerDel, Duration, false);

		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
	}
}

void AALOSpeedPotion::ResetSpeed(AAlchemistsLastDropCharacter* TargetChar, float OriginalSpeed)
{
	if (TargetChar && TargetChar->GetCharacterMovement())
	{
		TargetChar->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("Speed returned to normal."));
		}
	}

	Destroy();
}

FText AALOSpeedPotion::GetInteractableName_Implementation() const
{
	return FText::FromString(TEXT("Hız İksirini İç (E)"));
}