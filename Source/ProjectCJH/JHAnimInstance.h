// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Animation/AnimInstance.h"
#include "JHAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnApplyDamageDelegate);

UCLASS()
class PROJECTCJH_API UJHAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UJHAnimInstance();
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);
	void SetDeadAnim() { IsDead = true; }
public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnApplyDamageDelegate OnApplyDamage;

private:
	UFUNCTION()
	void AnimNotify_ApplyDamage();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	FName GetAttackMontageSectionName(int32 Section);
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn", Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn", Meta=(AllowPrivateAccess=true))
	float CurrentPawnDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn", Meta=(AllowPrivateAccess=true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn", Meta=(AllowPrivateAccess=true))
	UAnimMontage* AttackMontage;
};
