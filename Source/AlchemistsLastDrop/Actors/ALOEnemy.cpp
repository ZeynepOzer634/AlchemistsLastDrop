#include "Actors/ALOEnemy.h"
#include "Components/ALOHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

AALOEnemy::AALOEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	
	HealthComponent = CreateDefaultSubobject<UALOHealthComponent>(TEXT("HealthComponent"));

	
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
}

void AALOEnemy::BeginPlay()
{
	Super::BeginPlay();

	
	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AALOEnemy::HandleDeath);
	}

	
	GetWorld()->GetTimerManager().SetTimer(AIChaseTimerHandle, this, &AALOEnemy::ChasePlayer, 0.5f, true);
}

void AALOEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AALOEnemy::ChasePlayer()
{
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AAIController* AIController = Cast<AAIController>(GetController());

	if (PlayerPawn && AIController)
	{
		
		AIController->MoveToActor(PlayerPawn, 50.f);

		
		float DistanceToPlayer = FVector::Distance(GetActorLocation(), PlayerPawn->GetActorLocation());

		
		if (DistanceToPlayer <= 110.f)
		{
			UGameplayStatics::ApplyDamage(PlayerPawn, AttackDamage, GetController(), this, UDamageType::StaticClass());
		}
	}
}

void AALOEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
}

void AALOEnemy::HandleDeath(AActor* DeadActor)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Orange, TEXT("Enemy Defeated! Points added."));
	}

	
	GetWorld()->GetTimerManager().ClearTimer(AIChaseTimerHandle);
	Destroy();
}