#pragma once

#include "ProjectCJH.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveAlongPatrolRoute.generated.h"

class AJHAIController;
class AJHEnemyBase;
class AJHPatrolRoute;
struct FPathFollowingResult;
struct FAIRequestID;

UCLASS()
class PROJECTCJH_API UBTTask_MoveAlongPatrolRoute : public UBTTaskNode
{
    GENERATED_BODY()
public:
    UBTTask_MoveAlongPatrolRoute();
public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
    void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

private:
    TWeakObjectPtr<UBehaviorTreeComponent> CurrentOwnerComp;
};