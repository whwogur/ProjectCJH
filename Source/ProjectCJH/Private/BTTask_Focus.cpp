// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Focus.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_Focus::UBTTask_Focus()
{
    NodeName = "Focus";
    bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_Focus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        return EBTNodeResult::Failed;
    }

    AActor* FocusActor = Cast<AActor>(BlackboardComp->GetValueAsObject(FocusTarget.SelectedKeyName));
    if (FocusActor)
    {
        if (FocusActor->IsValidLowLevel())
        {
            // ��ȿ�� ���Ϳ� ��Ŀ�� ����
            AIController->SetFocus(FocusActor);
        }
        else
        {
            return EBTNodeResult::Failed;
        }
    }
    else
    {
        FVector FocusVector = BlackboardComp->GetValueAsVector(FocusTarget.SelectedKeyName);

        // ��ȿ�� �Ǵ�
        if (FocusVector.ContainsNaN() || FocusVector.IsNearlyZero())
        {
            return EBTNodeResult::Failed;
        }

        AIController->SetFocalPoint(FocusVector);
    }

    return EBTNodeResult::Succeeded;
}