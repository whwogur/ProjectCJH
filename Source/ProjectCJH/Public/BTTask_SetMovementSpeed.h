// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EMovementSpeed.h"
#include "BTTask_SetMovementSpeed.generated.h"

UCLASS()
class PROJECTCJH_API UBTTask_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetMovementSpeed();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	EMovementSpeed MovementSpeed;
};
