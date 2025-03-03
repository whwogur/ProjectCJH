// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/Character.h"
#include "JHCharacter.generated.h"

class UJHInputData;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class PROJECTCJH_API AJHCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJHCharacter();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Jump() override;
	virtual void StopJumping() override;
	virtual void AddControllerYawInput(float Val) override;
	virtual void AddControllerPitchInput(float Val) override;
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce = false) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	bool CanSetWeapon();
	void SetWeapon(class AJHWeapon* NewWeapon);
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnhancedInput")
	UJHInputData* InputActions;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class AJHWeapon* CurrentWeapon;
private:
	void OnJumpAction(const FInputActionValue& Value);
	void OnLookAction(const FInputActionValue& Value);
	void OnMoveAction(const FInputActionValue& Value);
	void OnAttackAction(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	void AttackStartComboState();
	void AttackEndComboState();
	void ApplyDamage();
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Attack", Meta=(AllowPrivateAccess=true))
	bool IsAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool CanExecuteNextCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool IsComboInputOn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 CurrentCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	int32 MaxCombo;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRadius;
	
	UPROPERTY()
	class UJHAnimInstance* JHAnim;
};
