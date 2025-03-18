// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Components/ActorComponent.h"
#include "JHCombatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

class AJHWeapon;

USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_BODY()

public:
	FAttackInfo()
		: Damage(0)
		, Radius(0)
		, Range(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Range;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCJH_API UJHCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJHCombatComponent();

public:
	void ApplyDamage(const FAttackInfo& AttackInfo);

public:
	bool CanSetWeapon();
	void SetWeapon(AJHWeapon* Weapon);
	void SetWeaponMeshVisible(bool Visibility, bool PropagateToChildren = false);

	AJHWeapon* GetCurWeapon() const { return CurrentWeapon; }
	FOnAttackEndDelegate OnAttackEnd;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AJHWeapon* CurrentWeapon;

public:
	bool IsAttacking() { return bAttacking; }
	void SetAttacking(bool Value) { bAttacking = Value; }

	UFUNCTION(BlueprintCallable)
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack", Meta = (AllowPrivateAccess = true))
	bool bAttacking;
};
