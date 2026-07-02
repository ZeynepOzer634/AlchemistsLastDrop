#include "AlchemistsLastDropCharacter.h"

#include "AlchemistsLastDrop.h"
#include "AlchemistsLastDropGameMode.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

AAlchemistsLastDropCharacter::AAlchemistsLastDropCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	InventoryComponent = CreateDefaultSubobject<UALOInventoryComponent>(TEXT("InventoryComponent"));
	HealthComponent = CreateDefaultSubobject<UALOHealthComponent>(TEXT("HealthComponent"));
	InteractionComponent = CreateDefaultSubobject<UALOInteractionComponent>(TEXT("InteractionComponent"));
}

void AAlchemistsLastDropCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AAlchemistsLastDropCharacter::HandlePlayerDeath);
	}
}

void AAlchemistsLastDropCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}

		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAlchemistsLastDropCharacter::Move);
		}

		if (MouseLookAction)
		{
			EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AAlchemistsLastDropCharacter::Look);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AAlchemistsLastDropCharacter::Look);
		}

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AAlchemistsLastDropCharacter::HandleInteract);
		}

		if (DashAction)
		{
			EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &AAlchemistsLastDropCharacter::HandleDash);
		}

		if (AttackAction)
		{
			EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AAlchemistsLastDropCharacter::HandleAttack);
		}
	}
	else
	{
		UE_LOG(LogAlchemistsLastDrop, Error, TEXT("Enhanced Input component not found on %s"), *GetNameSafe(this));
	}
}

void AAlchemistsLastDropCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	DoMove(MovementVector.X, MovementVector.Y);
}

void AAlchemistsLastDropCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AAlchemistsLastDropCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AAlchemistsLastDropCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController())
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AAlchemistsLastDropCharacter::DoJumpStart()
{
	Jump();
}

void AAlchemistsLastDropCharacter::DoJumpEnd()
{
	StopJumping();
}

void AAlchemistsLastDropCharacter::HandleInteract()
{
	if (InteractionComponent)
	{
		InteractionComponent->TryInteract();
	}
}

void AAlchemistsLastDropCharacter::HandleDash()
{
	FVector DashDirection = GetCharacterMovement()->Velocity.GetSafeNormal2D();

	if (DashDirection.IsNearlyZero())
	{
		DashDirection = GetActorForwardVector();
	}

	const FVector LaunchVelocity = DashDirection * DashStrength;
	LaunchCharacter(LaunchVelocity, true, false);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.5f,
			FColor::Purple,
			TEXT("DASH ACTIVATED!")
		);
	}
}

void AAlchemistsLastDropCharacter::HandleAttack()
{
	if (CurrentHeldPotion == EALOPotion::None)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Red,
				TEXT("Attack: Melee Punch")
			);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Yellow,
				FString::Printf(TEXT("Attack: Threw %s"), *UEnum::GetValueAsString(CurrentHeldPotion))
			);
		}

		CurrentHeldPotion = EALOPotion::None;
	}
}

void AAlchemistsLastDropCharacter::HandlePlayerDeath(AActor* DeadActor)
{
	if (GetWorld())
	{
		AAlchemistsLastDropGameMode* GM = Cast<AAlchemistsLastDropGameMode>(GetWorld()->GetAuthGameMode());

		if (GM)
		{
			GM->EndGame(false);
		}
	}
}

UALOInventoryComponent* AAlchemistsLastDropCharacter::GetInventoryComponent() const
{
	return InventoryComponent;
}

UALOHealthComponent* AAlchemistsLastDropCharacter::GetHealthComponent() const
{
	return HealthComponent;
}

UALOInteractionComponent* AAlchemistsLastDropCharacter::GetInteractionComponent() const
{
	return InteractionComponent;
}