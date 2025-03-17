// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateDistanceToTarget.generated.h"

UCLASS()
class PROJECTCJH_API UBTS_UpdateDistanceToTarget : public UBTService
{
	GENERATED_BODY()
public:
    UBTS_UpdateDistanceToTarget();

    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
    FBlackboardKeySelector DistanceToTargetKey;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
    FBlackboardKeySelector AttackTargetKey;
};
