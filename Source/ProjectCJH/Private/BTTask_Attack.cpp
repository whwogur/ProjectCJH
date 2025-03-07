// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "JHAIController.h"
#include "JHICombat.h"
#include "JHCombatComponent.h"
#include "JHCharacter.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    AJHCharacter* Character = Cast<AJHCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (!Character)
    {
        return EBTNodeResult::Failed;
    }

    UJHCombatComponent* CombatComp = Cast<UJHCombatComponent>(Character->GetComponentByClass(UJHCombatComponent::StaticClass()));
    if (!CombatComp)
    {
        return EBTNodeResult::Failed;
    }

    IJHICombat* CombatInterface = Cast<IJHICombat>(Character);
    if (!CombatInterface)
    {
        return EBTNodeResult::Failed;
    }

    CombatInterface->Attack();

    // BehaviorTreeComponent에 대한 포인터를 캡처합니다.
    UBehaviorTreeComponent* BTComp = &OwnerComp;
    CombatComp->OnAttackEnd.AddLambda([this, BTComp]() -> void {
        FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
    });

    return EBTNodeResult::InProgress;
}
