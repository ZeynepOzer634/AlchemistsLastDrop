#include "Actors/ALOFreezeTrap.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Actors/ALOEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

AALOFreezeTrap::AALOFreezeTrap()
{
	PrimaryActorTick.bCanEverTick = false;

	
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));

	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
}

void AALOFreezeTrap::BeginPlay()
{
	Super::BeginPlay();

	
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AALOFreezeTrap::OnTrapOverlap);
	}
}

void AALOFreezeTrap::OnTrapOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	
	AALOEnemy* Enemy = Cast<AALOEnemy>(OtherActor);
	if (Enemy && Enemy->GetCharacterMovement())
	{
		float OriginalSpeed = Enemy->GetCharacterMovement()->MaxWalkSpeed;

		
		Enemy->GetCharacterMovement()->MaxWalkSpeed = 0.f;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, FreezeDuration, FColor::Blue, TEXT("ENEMY FROZEN BY TRAP!"));
		}

		
		FTimerDelegate TimerDel;
		TimerDel.BindUObject(this, &AALOFreezeTrap::UnfreezeEnemy, Enemy, OriginalSpeed);
		GetWorld()->GetTimerManager().SetTimer(FreezeTimerHandle, TimerDel, FreezeDuration, false);

		
		SetActorHiddenInGame(true);
		TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AALOFreezeTrap::UnfreezeEnemy(AALOEnemy* TargetEnemy, float OriginalSpeed)
{
	if (TargetEnemy && TargetEnemy->GetCharacterMovement())
	{
		TargetEnemy->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("Enemy unfrozen."));
		}
	}

	
	Destroy();
}

