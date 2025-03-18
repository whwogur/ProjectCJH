// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EquipWeapon.generated.h"

UCLASS()
class PROJECTCJH_API UBTTask_EquipWeapon : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UBTTask_EquipWeapon();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    TWeakObjectPtr<class AJHEnemyBase> EnemyWeakPtr;
    FDelegateHandle DelegateHandle;
};

UCLASS()
class PROJECTCJH_API UBTTask_SheatheWeapon : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UBTTask_SheatheWeapon();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    TWeakObjectPtr<class AJHEnemyBase> EnemyWeakPtr;
    FDelegateHandle DelegateHandle;
};