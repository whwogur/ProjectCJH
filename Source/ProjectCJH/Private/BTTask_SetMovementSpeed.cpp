// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetMovementSpeed.h"
#include "JHAIController.h"

UBTTask_SetMovementSpeed::UBTTask_SetMovementSpeed()
{
	NodeName = "Set Movement Speed";
}

EBTNodeResult::Type UBTTask_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AJHAIController* AIController = Cast<AJHAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    AIController->SetMovementSpeed(MovementSpeed);

    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    return EBTNodeResult::Succeeded;
}
