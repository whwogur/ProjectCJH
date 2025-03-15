// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMovementSpeed.generated.h"

UENUM(BlueprintType)
enum class EMovementSpeed : uint8
{
	IDLE			UMETA(DisplayName = "Idle"),
	WALK			UMETA(DisplayName = "Walk"),
	JOG				UMETA(DisplayName = "Jog"),
	SPRINT			UMETA(DisplayName = "Sprint")
};

// Behavior Tree���� ��� �����ϵ��� ����ü ����
USTRUCT(BlueprintType)
struct FBTMovementSpeed
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    EMovementSpeed Value;

    FBTMovementSpeed() { Value = EMovementSpeed::IDLE; }
};