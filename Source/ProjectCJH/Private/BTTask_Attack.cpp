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
    if (!OwnerComp.GetAIOwner())
    {
        return EBTNodeResult::Failed;
    }

    // AI�� �����ϴ� ĳ���� ��������
    AJHCharacter* Character = Cast<AJHCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (!Character)
    {
        return EBTNodeResult::Failed;
    }

    // Combat ������Ʈ ��������
    UJHCombatComponent* CombatComp = Character->FindComponentByClass<UJHCombatComponent>();
    if (!CombatComp)
    {
        return EBTNodeResult::Failed;
    }

    // Combat �������̽� Ȯ�� �� ���� ����
    if (IJHICombat* CombatInterface = Cast<IJHICombat>(Character))
    {
        CombatInterface->Attack();
    }
    else
    {
        return EBTNodeResult::Failed;
    }

    // BehaviorTreeComponent ������ �����ϰ� ĸó
    CombatComp->OnAttackEnd.AddWeakLambda(this, [this, &OwnerComp]()
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        });

    return EBTNodeResult::InProgress;
}
