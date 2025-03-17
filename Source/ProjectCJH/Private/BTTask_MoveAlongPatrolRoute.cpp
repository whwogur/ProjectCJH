#include "BTTask_MoveAlongPatrolRoute.h"
#include "Navigation/PathFollowingComponent.h"
#include "JHPatrolRoute.h"
#include "JHAIController.h"
#include "JHEnemyBase.h"

UBTTask_MoveAlongPatrolRoute::UBTTask_MoveAlongPatrolRoute()
{
    NodeName = "Move Along Patrol Route";
}

// Task 실행 로직
EBTNodeResult::Type UBTTask_MoveAlongPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AI 컨트롤러 가져오기
    AJHAIController* AIController = Cast<AJHAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        JHLOG(Warning, TEXT("BTTask_MoveAlongPatrolRoute: AIController is null"));
        return EBTNodeResult::Failed;
    }

    // 제어 중인 Pawn 가져오기
    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn)
    {
        JHLOG(Warning, TEXT("BTTask_MoveAlongPatrolRoute: ControlledPawn is null"));
        return EBTNodeResult::Failed;
    }

    AJHEnemyBase* Enemy = Cast<AJHEnemyBase>(ControlledPawn);
    if (!Enemy)
    {
        JHLOG(Warning, TEXT("BTTask_MoveAlongPatrolRoute: Failed Enemy Cast"));
        return EBTNodeResult::Failed;
    }

    // 순찰 경로 가져오기
    AJHPatrolRoute* PatrolRoute = Enemy->GetPatrolRoute();
    if (!PatrolRoute)
    {
        JHLOG(Error, TEXT("BTTask_MoveAlongPatrolRoute: PatrolRoute is null"));
        return EBTNodeResult::Failed;
    }

    FVector Destination = PatrolRoute->GetSplinePointAsWorldPosition();

    // AI를 목적지로 이동
    FAIMoveRequest MoveRequest(Destination);
    MoveRequest.SetAcceptanceRadius(30.0f);
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetProjectGoalLocation(true);
    MoveRequest.SetAllowPartialPath(true);

    FPathFollowingRequestResult Result = AIController->MoveTo(MoveRequest);
    if (Result.Code == EPathFollowingRequestResult::RequestSuccessful)
    {
        // 이동 성공 -> ID 가져오기
        FAIRequestID RequestID = Result.MoveId;
        if (RequestID.IsValid())
        {
            CurrentOwnerComp = &OwnerComp;
            AIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UBTTask_MoveAlongPatrolRoute::OnMoveCompleted);
            return EBTNodeResult::InProgress;
        }
    }
    JHLOG(Warning, TEXT("BTTask_MoveAlongPatrolRoute: MoveTo failed to start"));
    return EBTNodeResult::Failed;
}

// Task 중단 로직
EBTNodeResult::Type UBTTask_MoveAlongPatrolRoute::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (AIController)
    {
        // 이동 중지
        AIController->StopMovement();

        // 콜백 제거
        if (AIController->GetPathFollowingComponent())
        {
            AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
        }
    }

    CurrentOwnerComp.Reset();
    return EBTNodeResult::Aborted;
}

// 이동 완료 콜백
void UBTTask_MoveAlongPatrolRoute::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
    if (!CurrentOwnerComp.IsValid())
    {
        JHLOG(Warning, TEXT("BTTask_MoveAlongPatrolRoute: OwnerComp is invalid in OnMoveCompleted"));
        return;
    }

    AAIController* AIController = Cast<AAIController>(CurrentOwnerComp->GetAIOwner());
    if (!AIController)
    {
        JHLOG(Warning, TEXT("BTTask_MoveAlongPatrolRoute: AIController is null in OnMoveCompleted"));
        FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Failed);
        return;
    }

    APawn* ControlledPawn = AIController->GetPawn();
    if (!ControlledPawn)
    {
        JHLOG(Warning, TEXT("BTTask_MoveAlongPatrolRoute: ControlledPawn is null in OnMoveCompleted"));
        FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Failed);
        return;
    }

    AJHEnemyBase* Enemy = Cast<AJHEnemyBase>(ControlledPawn);
    if (!Enemy)
    {
        JHLOG(Warning, TEXT("BTTask_MoveAlongPatrolRoute: Failed Enemy Cast"));
        FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Failed);
        return;
    }

    AJHPatrolRoute* PatrolRoute = Enemy->GetPatrolRoute();
    if (!PatrolRoute)
    {
        JHLOG(Warning, TEXT("BTTask_MoveAlongPatrolRoute: PatrolRoute is null in OnMoveCompleted"));
        FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Failed);
        return;
    }

    // 이동 결과 처리
    if (Result.Code == EPathFollowingResult::Success || Result.Code == EPathFollowingResult::Aborted)
    {
        // 다음 순찰 지점으로 인덱스 증가
        PatrolRoute->IncrementPatrolRoute();
        FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
    }
    else
    {
        FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Failed);
    }

    // 콜백 제거 및 정리
    if (AIController->GetPathFollowingComponent())
    {
        AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
    }
    CurrentOwnerComp.Reset();
}