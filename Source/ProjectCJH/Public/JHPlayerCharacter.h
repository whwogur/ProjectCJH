// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "JHCharacter.h"
#include "JHICombat.h"
#include "JHPlayerCharacter.generated.h"

class UJHInputData;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class PROJECTCJH_API AJHPlayerCharacter : public AJHCharacter, public IJHICombat
{
	GENERATED_BODY()
	
public:
	AJHPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce = false) override;

// =================
// Combat Interface
// =================
public:
	virtual void SetWeapon(AJHWeapon* NewWeapon, const FName& SocketName) override;
	virtual void Attack() override;
	virtual void Die() override;
	
	virtual bool CanSetWeapon();
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnAssetLoadCompleted() override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UJHInputData* InputActions;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	class UJHCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UJHCombatComponent* Combat;
private:
	void OnJumpAction(const FInputActionValue& Value);
	void OnLookAction(const FInputActionValue& Value);
	void OnMoveAction(const FInputActionValue& Value);
	void OnAttackAction(const FInputActionValue& Value);
	//void OnAssetLoadCompleted();

	void AttackStartComboState();
	void AttackEndComboState();

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool CanExecuteNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY()
	class AJHPlayerController* JHPlayerController;

private:
	static const FName WeaponSocketName;
};
