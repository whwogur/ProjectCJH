// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_Enemy.h"
#include "JHAIController.h"
#include "JHCharacter.h"
#include "BehaviorTree\BlackboardComponent.h"
#include "JHEnemyBase.h"

// ==================
// BTD IsInAttackRange
// ==================
UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("Is In AttackRange w AttackTarget");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
		return false;

	AJHCharacter* AttackTarget = Cast<AJHCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AJHAIController::AttackTargetKey));
	if (!AttackTarget)
		return false;

	float AttackRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AJHAIController::AttackRadiusKey);

	return (AttackTarget->GetDistanceTo(ControllingPawn) <= AttackRadius);
}


// ==================
// BTD HasPatrolRoute
// ==================
UBTDecorator_HasPatrolRoute::UBTDecorator_HasPatrolRoute()
{
	NodeName = TEXT("Has Patrol Route");
}

bool UBTDecorator_HasPatrolRoute::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
		return false;
	AJHEnemyBase* ControllingEnemy = Cast<AJHEnemyBase>(ControllingPawn);
	if (!ControllingEnemy)
		return false;

	return (nullptr != ControllingEnemy->GetPatrolRoute());
}


// ====================
// BTD IsWeaponEquipped
// ====================
UBTDecorator_IsWeaponEquipped::UBTDecorator_IsWeaponEquipped()
{
	NodeName = TEXT("Is Weapon Equipped");
}

bool UBTDecorator_IsWeaponEquipped::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
		return false;
	AJHEnemyBase* ControllingEnemy = Cast<AJHEnemyBase>(ControllingPawn);
	if (!ControllingEnemy)
		return false;

	bool weaponEqupped = ControllingEnemy->IsWeaponEquipped();

	//JHLOG(Warning, TEXT("WeaponEquipped %s"), weaponEqupped ? TEXT("true") : TEXT("false"));
	return weaponEqupped;
}

// ==================
// BTD IsInDefenseRange
// ==================
UBTDecorator_IsInDefenseRange::UBTDecorator_IsInDefenseRange()
{
	NodeName = TEXT("Is In DefenseRange w AttackTarget");
}

bool UBTDecorator_IsInDefenseRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!ControllingPawn)
		return false;

	AJHCharacter* AttackTarget = Cast<AJHCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AJHAIController::AttackTargetKey));
	if (!AttackTarget)
		return false;

	float DefenseRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AJHAIController::DefendRadiusKey);

	return (AttackTarget->GetDistanceTo(ControllingPawn) <= DefenseRadius);
}
