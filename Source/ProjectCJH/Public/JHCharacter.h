// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/Character.h"
#include "JHCharacter.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	// Called to bind functionality to input
	virtual void Attack();

	UFUNCTION(BlueprintCallable)
	virtual void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	bool CanSetWeapon();
	void SetWeapon(class AJHWeapon* NewWeapon);

	FOnAttackEndDelegate OnAttackEnd;
public:
	bool IsAttacking() { return bAttacking; }
	void SetAttacking(bool Value) { bAttacking = Value; }
public:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class AJHWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	class UJHCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UJHCombatComponent* Combat;
private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bAttacking;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackRadius;
private:
	UPROPERTY()
	class UJHAnimInstance* JHAnim;

	FSoftObjectPath CharacterAssetToLoad;
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

};
