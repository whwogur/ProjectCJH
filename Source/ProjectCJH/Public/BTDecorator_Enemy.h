// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_Enemy.generated.h"

UCLASS()
class PROJECTCJH_API UBTDecorator_IsInAttackRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsInAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

UCLASS()
class PROJECTCJH_API UBTDecorator_IsWeaponEquipped : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsWeaponEquipped();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

UCLASS()
class PROJECTCJH_API UBTDecorator_HasPatrolRoute : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_HasPatrolRoute();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};

UCLASS()
class PROJECTCJH_API UBTDecorator_IsInDefenseRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsInDefenseRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};