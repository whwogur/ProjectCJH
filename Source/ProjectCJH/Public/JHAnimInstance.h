// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Animation/AnimInstance.h"
#include "JHAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnApplyDamageDelegate);
DECLARE_MULTICAST_DELEGATE(FOnPullOutDelegate);
DECLARE_MULTICAST_DELEGATE(FOnSheatheDelegate);

UCLASS()
class PROJECTCJH_API UJHAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UJHAnimInstance();
	
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	void SetDeadAnim() { IsDead = true; }
public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnApplyDamageDelegate OnApplyDamage;
	FOnPullOutDelegate OnPullOut;
	FOnSheatheDelegate OnSheathe;
private:
	UFUNCTION()
	void AnimNotify_ApplyDamage();

	UFUNCTION()
	void AnimNotify_NextAttackCheck();

	UFUNCTION()
	void AnimNotify_PullOut();

	UFUNCTION()
	void AnimNotify_Sheathe();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn", Meta=(AllowPrivateAccess=true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn", Meta=(AllowPrivateAccess=true))
	float CurrentPawnDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn", Meta=(AllowPrivateAccess=true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool IsDead;

};
