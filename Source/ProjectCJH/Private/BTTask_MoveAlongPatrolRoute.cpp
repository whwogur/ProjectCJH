#include "BTTask_MoveAlongPatrolRoute.h"
#include "Navigation/PathFollowingComponent.h"
#include "JHPatrolRoute.h"
#include "JHAIController.h"
#include "JHEnemyBase.h"

UBTTask_MoveAlongPatrolRoute::UBTTask_MoveAlongPatrolRoute()
{
    NodeName = "Move Along Patrol Route";
}

// Task ���� ����
EBTNodeResult::Type UBTTask_MoveAlongPatrolRoute::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AI ��Ʈ�ѷ� ��������
    AJHAIController* AIController = Cast<AJHAIController>(OwnerComp.GetAIOwner());
    if (!AIController)
    {
        JHLOG(Warning, TEXT("BTTask_MoveAlongPatrolRoute: AIController is null"));
        return EBTNodeResult::Failed;
    }

    // ���� ���� Pawn ��������
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

    // ���� ��� ��������
    AJHPatrolRoute* PatrolRoute = Enemy->GetPatrolRoute();
    if (!PatrolRoute)
    {
        JHLOG(Error, TEXT("BTTask_MoveAlongPatrolRoute: PatrolRoute is null"));
        return EBTNodeResult::Failed;
    }

    FVector Destination = PatrolRoute->GetSplinePointAsWorldPosition();

    // AI�� �������� �̵�
    FAIMoveRequest MoveRequest(Destination);
    MoveRequest.SetAcceptanceRadius(30.0f);
    MoveRequest.SetUsePathfinding(true);
    MoveRequest.SetProjectGoalLocation(true);
    MoveRequest.SetAllowPartialPath(true);

    FPathFollowingRequestResult Result = AIController->MoveTo(MoveRequest);
    if (Result.Code == EPathFollowingRequestResult::RequestSuccessful)
    {
        // �̵� ���� -> ID ��������
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

// Task �ߴ� ����
EBTNodeResult::Type UBTTask_MoveAlongPatrolRoute::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = Cast<AAIController>(OwnerComp.GetAIOwner());
    if (AIController)
    {
        // �̵� ����
        AIController->StopMovement();

        // �ݹ� ����
        if (AIController->GetPathFollowingComponent())
        {
            AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
        }
    }

    CurrentOwnerComp.Reset();
    return EBTNodeResult::Aborted;
}

// �̵� �Ϸ� �ݹ�
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

    // �̵� ��� ó��
    if (Result.Code == EPathFollowingResult::Success || Result.Code == EPathFollowingResult::Aborted)
    {
        // ���� ���� �������� �ε��� ����
        PatrolRoute->IncrementPatrolRoute();
        FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Succeeded);
    }
    else
    {
        FinishLatentTask(*CurrentOwnerComp, EBTNodeResult::Failed);
    }

    // �ݹ� ���� �� ����
    if (AIController->GetPathFollowingComponent())
    {
        AIController->GetPathFollowingComponent()->OnRequestFinished.RemoveAll(this);
    }
    CurrentOwnerComp.Reset();
}