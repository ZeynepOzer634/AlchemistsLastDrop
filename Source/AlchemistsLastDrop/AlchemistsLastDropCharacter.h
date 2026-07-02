#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"

#include "Core/ALOCoreTypes.h"
#include "Components/ALOInventoryComponent.h"
#include "Components/ALOHealthComponent.h"
#include "Components/ALOInteractionComponent.h"

#include "AlchemistsLastDropCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(abstract)
class ALCHEMISTSLASTDROP_API AAlchemistsLastDropCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AAlchemistsLastDropCharacter();

protected:

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void HandleInteract();
	void HandleDash();
	void HandleAttack();

	UFUNCTION()
	void HandlePlayerDeath(AActor* DeadActor);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ALO|Movement")
	float DashStrength = 2500.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALO|Gameplay")
	EALOPotion CurrentHeldPotion = EALOPotion::None;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALO|Components", meta = (AllowPrivateAccess = "true"))
	UALOInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALO|Components", meta = (AllowPrivateAccess = "true"))
	UALOHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALO|Components", meta = (AllowPrivateAccess = "true"))
	UALOInteractionComponent* InteractionComponent;

public:

	UFUNCTION(BlueprintCallable, Category = "ALO|Player")
	UALOInventoryComponent* GetInventoryComponent() const;

	UFUNCTION(BlueprintCallable, Category = "ALO|Player")
	UALOHealthComponent* GetHealthComponent() const;

	UFUNCTION(BlueprintCallable, Category = "ALO|Player")
	UALOInteractionComponent* GetInteractionComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable, Category = "ALO|Player")
	EALOPotion GetCurrentHeldPotion() const { return CurrentHeldPotion; }

	UFUNCTION(BlueprintCallable, Category = "ALO|Player")
	void SetCurrentHeldPotion(EALOPotion NewPotion) { CurrentHeldPotion = NewPotion; }
};