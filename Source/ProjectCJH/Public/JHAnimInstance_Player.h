// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHAnimInstance.h"
#include "JHAnimInstance_Player.generated.h"
DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnComboAttackEndDelegate);

UCLASS()
class PROJECTCJH_API UJHAnimInstance_Player : public UJHAnimInstance
{
	GENERATED_BODY()
public:
	UJHAnimInstance_Player();
public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnComboAttackEndDelegate OnComboAttackEnd;

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	void JumpToAttackMontageSection(int32 NewSection, const UAnimMontage* Montage);
	FName GetAttackMontageSectionName(int32 Section);
};
