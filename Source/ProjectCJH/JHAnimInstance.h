// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Animation/AnimInstance.h"
#include "JHAnimInstance.generated.h"

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
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Pawn", Meta=(AllowPrivateAccess = true))
	float CurrentPawnSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	float CurrentPawnDirection;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	bool IsInAir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
};
