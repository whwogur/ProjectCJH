// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FaceTarget.generated.h"

UCLASS()
class PROJECTCJH_API UBTTask_FaceTarget : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FaceTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
