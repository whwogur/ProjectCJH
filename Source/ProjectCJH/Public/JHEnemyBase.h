// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHCharacter.h"
#include "JHICombat.h"
#include "JHEnemyBase.generated.h"

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
	virtual void SetWeapon(AJHWeapon* NewWeapon) override;
	virtual void Attack() override;
	virtual void Die() override;

	virtual bool CanSetWeapon();

public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnAssetLoadCompleted() override;

public:
	int32 GetExp() const;
public:
	UPROPERTY(VisibleAnywhere, Category = "UI")
	class UWidgetComponent* HPBarWidget;

	UPROPERTY(VisibleAnywhere, Category = "Stat")
	class UJHCharacterStatComponent* CharacterStat;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UJHCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	class UWidgetComponent* CombatIndicator;
private:
	UPROPERTY()
	class AJHAIController* JHAIController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State", Meta = (AllowPrivateAccess = true))
	float DeadTimer;

	FTimerHandle DeadTimerHandle;

	UPROPERTY()
	class UJHCombatIndicator* CombatIndicatorUI;
};
