// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Components/ActorComponent.h"
#include "JHCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangedDelegate);

UCLASS()
class PROJECTCJH_API UJHCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UJHCharacterStatComponent();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:
	void SetNewLevel(int32 NewLevel);
	void SetDamageReceived(float NewDamage);
	void SetHP(float NewHP);

	float GetAttack();
	float GetHPRatio();
	int32 GetDropExp() const;

	FOnHPIsZeroDelegate OnHPIsZero;
	FOnHPChangedDelegate OnHPChanged;

private:
	struct FJHCharacterData* CurrentStatData;

	UPROPERTY(EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP;
	// TODO - STATS
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	//float AttackDamageMin;
	//
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	//float AttackDamageMax;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackModifierMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", Meta = (AllowPrivateAccess = true))
	float AttackModifierMax;
};
