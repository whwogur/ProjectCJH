// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearFocus.h"
#include "JHAIController.h"

UBTTask_ClearFocus::UBTTask_ClearFocus()
{
    NodeName = "Clear Focus";
}

EBTNodeResult::Type UBTTask_ClearFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AJHAIController* AIController = Cast<AJHAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AIController->ClearFocus(EAIFocusPriority::Gameplay);

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}