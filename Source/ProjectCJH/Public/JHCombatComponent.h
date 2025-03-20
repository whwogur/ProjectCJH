// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Components/ActorComponent.h"
#include "JHCombatComponent.generated.h"

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
		, CanBeDodged(true)
		, CanBeBlocked(true)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Range;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool CanBeDodged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool CanBeBlocked;
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
	void ApplyAOEDamage(const FAttackInfo& AttackInfo);
public:
	bool CanSetWeapon();
	void SetWeapon(AJHWeapon* Weapon);
	void SetWeaponMeshVisible(bool Visibility, bool PropagateToChildren = false);

	AJHWeapon* GetCurWeapon() const { return CurrentWeapon; }
	

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AJHWeapon* CurrentWeapon;
};
