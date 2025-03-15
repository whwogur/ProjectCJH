// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Focus.generated.h"

UCLASS()
class PROJECTCJH_API UBTTask_Focus : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTTask_Focus();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    UPROPERTY(EditAnywhere, Category = "Blackboard")
    FBlackboardKeySelector FocusTarget;
};
