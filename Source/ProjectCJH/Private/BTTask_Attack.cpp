// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "JHAIController.h"
#include "JHCharacter.h"

UBTTask_Attack::UBTTask_Attack()
	: IsAttacking(false)
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AJHCharacter* Character = Cast<AJHCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Character)
		return EBTNodeResult::Failed;

	Character->Attack();
	IsAttacking = true;
	Character->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
