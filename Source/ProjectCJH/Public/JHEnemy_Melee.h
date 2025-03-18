// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "JHEnemyBase.h"
#include "JHEnemy_Melee.generated.h"



UCLASS()
class PROJECTCJH_API AJHEnemy_Melee : public AJHEnemyBase
{
	GENERATED_BODY()
	
public:
	AJHEnemy_Melee();

public:
	virtual void Attack() override;
	virtual void EquipWeapon() override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	virtual void OnAssetLoadCompleted() override;
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AJHWeapon> WeaponClass;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	UAnimMontage* EquipWeaponMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	UAnimMontage* SheatheWeaponMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation", Meta = (AllowPrivateAccess = true))
	UAnimMontage* DamagedMontage;


private:
	static const FName WeaponSocketName_Equipped;
	static const FName WeaponSocketName_Sheathed;


};
