// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Components/ActorComponent.h"
#include "JHCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);

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
	void SetDamage(float NewDamage);
	float GetAttack();

	FOnHPIsZeroDelegate OnHPIsZero;

private:
	struct FJHCharacterData* CurrentStatData;

	UPROPERTY(EditInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	int32 Level;

	// 직렬화에서 제외
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", Meta = (AllowPrivateAccess = true))
	float CurrentHP;
};
