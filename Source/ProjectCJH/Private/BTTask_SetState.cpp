// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetState.h"
#include "JHAIController.h"

UBTTask_SetState::UBTTask_SetState()
{
    NodeName = "Set State";
}

EBTNodeResult::Type UBTTask_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AJHAIController* AIController = Cast<AJHAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AIController->SetAIState(State.Value);

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}
