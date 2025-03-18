// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "JHCharacter.h"
#include "JHICombat.h"
#include "JHEnemyBase.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnWeaponEquippedDelegate);
DECLARE_MULTICAST_DELEGATE(FOnWeaponSheathedDelegate);

class AJHPatrolRoute;

UCLASS()
class PROJECTCJH_API AJHEnemyBase : public AJHCharacter, public IJHICombat
{
	GENERATED_BODY()
	
public:
	AJHEnemyBase();

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
// =================
// Combat Interface
// =================
public:
	virtual void SetWeapon(AJHWeapon* NewWeapon, const FName& SocketName) override;
	virtual void Attack() { JHLOG_S(Error); };
	virtual void Die() override;

	virtual bool CanSetWeapon();

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnAssetLoadCompleted() override;
	virtual void EquipWeapon() { JHLOG(Error, TEXT("Did Not override EqupWeapon but called by BT")); }
	virtual void SheatheWeapon() { JHLOG(Error, TEXT("Did Not override SheatheWeapon but called by BT")); }

	UFUNCTION()
	void OnWeaponEquipCompleted(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnWeaponSheatheCompleted(UAnimMontage* Montage, bool bInterrupted);
public:
	bool IsWeaponEquipped() const { return WeaponEquipped; }
public:
	int32 GetExp() const;
	AJHPatrolRoute* GetPatrolRoute();

protected:
	void MoveWeapon(const FName& SocketName);

public:
	FOnWeaponEquippedDelegate OnWeaponEquipped;
	FOnWeaponSheathedDelegate OnWeaponSheathed;

public:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	class UJHCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UJHCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UWidgetComponent* CombatIndicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Behavior")
	class AJHPatrolRoute* PatrolRoute;

protected:
	class UJHAnimInstance* EnemyAnimInstance;
private:
	UPROPERTY()
	class AJHAIController* JHAIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle;

	UPROPERTY()
	class UJHCombatIndicator* CombatIndicatorUI;

private:
	bool WeaponEquipped;
};
