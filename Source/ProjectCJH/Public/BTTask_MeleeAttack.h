// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MeleeAttack.generated.h"

enum class EMeleeAttack : uint8;

UCLASS()
class PROJECTCJH_API UBTTask_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTTask_MeleeAttack();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector AttackTargetKey;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector AttackRadiusKey;

    UPROPERTY(EditAnywhere, Category = "AttackType")
    EMeleeAttack AttackName;
};
