// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EEnemyState.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	PASSIVE			UMETA(DisplayName = "Passive"),
	ATTACK			UMETA(DisplayName = "Attack"),
	STUN			UMETA(DisplayName = "Stun"),
	INVESTIGATE		UMETA(DisplayName = "Investigate"),
	DEAD			UMETA(DisplayName = "Dead"),
	SEEK			UMETA(DisplayName = "Seek"),
};

// Behavior Tree에서 사용 가능하도록 구조체 래핑
USTRUCT(BlueprintType)
struct FBTEnemyState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	EEnemyState Value;

	FBTEnemyState() { Value = EEnemyState::PASSIVE; }
};
