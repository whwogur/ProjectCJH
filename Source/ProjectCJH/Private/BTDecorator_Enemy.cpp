// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Enemy.h"
#include "JHAIController.h"
#include "JHCharacter.h"
#include "BehaviorTree\BlackboardComponent.h"

// ==================
// BTD IsInAttackRange
// ==================
UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("IsInAttackRange");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	AJHCharacter* AttackTarget = Cast<AJHCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AJHAIController::AttackTargetKey));

	float AttackRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AJHAIController::AttackRadiusKey);

	if (nullptr == AttackTarget)
		return false;

	bResult = (AttackTarget->GetDistanceTo(ControllingPawn) <= AttackRadius);

	return bResult;
}


// ==================
// BTD HasPatrolRoute
// ==================
UBTDecorator_HasPatrolRoute::UBTDecorator_HasPatrolRoute()
{
	NodeName = TEXT("HasPatrolRoute");
}

bool UBTDecorator_HasPatrolRoute::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return false;
}


// ====================
// BTD IsWeaponEquipped
// ====================
UBTDecorator_IsWeaponEquipped::UBTDecorator_IsWeaponEquipped()
{
	NodeName = TEXT("IsWeaponEquipped");
}

bool UBTDecorator_IsWeaponEquipped::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return false;
}
#pragma endregion