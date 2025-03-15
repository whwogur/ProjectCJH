// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EEnemyState.h"
#include "BTTask_SetState.generated.h"

UCLASS()
class PROJECTCJH_API UBTTask_SetState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetState();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	FBTEnemyState State;
};
