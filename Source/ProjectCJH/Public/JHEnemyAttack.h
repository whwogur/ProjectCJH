// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "JHEnemyAttack.generated.h"

UENUM(BlueprintType)
enum class EMeleeAttack : uint8
{
	SHORTRANGED			UMETA(DisplayName = "ShortRangedAttack"),
	JUMPATTACK			UMETA(DisplayName = "JumpAttack"),
	SPINATTACK			UMETA(DisplayName = "SpinAttack"),
	GROUNDSMASH			UMETA(DisplayName = "GroundSmash"),
};

// Behavior Tree에서 사용 가능하도록 구조체 래핑
USTRUCT(BlueprintType)
struct FBTMeleeAttack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MeleeAttack")
	EMeleeAttack Value;

	FBTMeleeAttack() { Value = EMeleeAttack::SHORTRANGED; }
};

class PROJECTCJH_API JHEnemyAttack
{
public:
	JHEnemyAttack();
	~JHEnemyAttack();
};
