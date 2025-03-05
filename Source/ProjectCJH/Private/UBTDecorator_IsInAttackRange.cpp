// Fill out your copyright notice in the Description page of Project Settings.


#include "UBTDecorator_IsInAttackRange.h"
#include "JHAIController.h"
#include "JHCharacter.h"
#include "BehaviorTree\BlackboardComponent.h"

UUBTDecorator_IsInAttackRange::UUBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("IsInAttackRange");
}

bool UUBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	AJHCharacter* AttackTarget = Cast<AJHCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AJHAIController::TargetKey));

	if (nullptr == AttackTarget)
		return false;

	bResult = (AttackTarget->GetDistanceTo(ControllingPawn) <= 200.0f);

	return bResult;
}
