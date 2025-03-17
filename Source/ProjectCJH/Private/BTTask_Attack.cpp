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

    // AI가 조종하는 캐릭터 가져오기
    AJHCharacter* Character = Cast<AJHCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (!Character)
    {
        return EBTNodeResult::Failed;
    }

    // Combat 컴포넌트 가져오기
    UJHCombatComponent* CombatComp = Character->FindComponentByClass<UJHCombatComponent>();
    if (!CombatComp)
    {
        return EBTNodeResult::Failed;
    }

    // Combat 인터페이스 확인 후 공격 실행
    if (IJHICombat* CombatInterface = Cast<IJHICombat>(Character))
    {
        CombatInterface->Attack();
    }
    else
    {
        return EBTNodeResult::Failed;
    }

    // BehaviorTreeComponent 포인터 안전하게 캡처
    CombatComp->OnAttackEnd.AddWeakLambda(this, [this, &OwnerComp]()
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        });

    return EBTNodeResult::InProgress;
}
