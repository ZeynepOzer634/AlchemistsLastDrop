#include "AlchemistsLastDropGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "UObject/ConstructorHelpers.h"

AAlchemistsLastDropGameMode::AAlchemistsLastDropGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(
		TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter")
	);

	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AAlchemistsLastDropGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AAlchemistsLastDropGameMode::EndGame(bool bIsVictory)
{
	OnGameOver.Broadcast(bIsVictory);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PC)
	{
		PC->SetInputMode(FInputModeUIOnly());
		PC->bShowMouseCursor = true;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.0f,
			bIsVictory ? FColor::Green : FColor::Red,
			bIsVictory ? TEXT("GAME OVER: VICTORY!") : TEXT("GAME OVER: DEFEAT!")
		);
	}
}