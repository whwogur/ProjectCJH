// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToIdealRange.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Tasks/AITask_MoveTo.h"

// 생성자
UBTTask_MoveToIdealRange::UBTTask_MoveToIdealRange()
{
    NodeName = "Move To Ideal Range";
}

EBTNodeResult::Type UBTTask_MoveToIdealRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
    if (!Blackboard)
    {
        return EBTNodeResult::Failed;
    }

    AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(AttackTargetKey.SelectedKeyName));
    if (!TargetActor)
    {
        return EBTNodeResult::Failed;
    }

    // 허용 반경 가져오기
    float AcceptanceRadius = Blackboard->GetValueAsFloat(IdealRangeKey.SelectedKeyName);

    FAIMoveRequest MoveRequest;
    MoveRequest.SetGoalActor(TargetActor);
    MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
    MoveRequest.SetUsePathfinding(true);

    FPathFollowingRequestResult MoveResult = AIController->MoveTo(MoveRequest);
    if (MoveResult.Code == EPathFollowingRequestResult::Failed)
    {
        return EBTNodeResult::Failed;
    }

    TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompWeakPtr = &OwnerComp;

    // OnRequestFinished 에 람다 바인딩
    AIController->GetPathFollowingComponent()->OnRequestFinished.AddLambda(
        [this, OwnerCompWeakPtr](FAIRequestID RequestID, const FPathFollowingResult& Result)
        {
            if (OwnerCompWeakPtr.IsValid())
            {
                EBTNodeResult::Type TaskResult = Result.IsSuccess() ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
                FinishLatentTask(*OwnerCompWeakPtr, TaskResult);
            }
        }
    );

    return EBTNodeResult::InProgress;
}