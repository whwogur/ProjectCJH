// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHAnimInstance.h"
#include "JHAnimInstance_Player.generated.h"

UCLASS()
class PROJECTCJH_API UJHAnimInstance_Player : public UJHAnimInstance
{
	GENERATED_BODY()
public:
	UJHAnimInstance_Player();
public:
	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);
	FName GetAttackMontageSectionName(int32 Section);
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pawn", Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;
};
