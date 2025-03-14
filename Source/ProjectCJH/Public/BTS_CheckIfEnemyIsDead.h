// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckIfEnemyIsDead.generated.h"

UCLASS()
class PROJECTCJH_API UBTS_CheckIfEnemyIsDead : public UBTService
{
	GENERATED_BODY()
	
public:
    UBTS_CheckIfEnemyIsDead();

    // Override TickNode to implement the service logic
    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    // Blackboard key to retrieve the attack target, editable in the editor
    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector AttackTargetKey;
};
