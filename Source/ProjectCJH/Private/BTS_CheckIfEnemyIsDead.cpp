// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CheckIfEnemyIsDead.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "JHCharacter.h"
#include "JHAIController.h"

UBTS_CheckIfEnemyIsDead::UBTS_CheckIfEnemyIsDead()
{
	NodeName = "Check If Enemy Is Dead";
    Interval = 1.0f;
}

void UBTS_CheckIfEnemyIsDead::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return;
    }

    AJHCharacter* AttackTarget = Cast<AJHCharacter>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
    if (!AttackTarget)
    {
        return;
    }

    if (AttackTarget && ECharacterState::DEAD == AttackTarget->GetCharacterState())
    {
        AAIController* AIController = OwnerComp.GetAIOwner();
        AJHAIController* thisAIController = Cast<AJHAIController>(AIController);
        if (thisAIController)
        {
            thisAIController->SetAIState(EEnemyState::PASSIVE);
        }
    }
}
