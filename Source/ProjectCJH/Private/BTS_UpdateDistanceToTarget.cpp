// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdateDistanceToTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UBTS_UpdateDistanceToTarget::UBTS_UpdateDistanceToTarget()
{
	NodeName = "Update Distance To Target";
	Interval = 1.0f;
    AttackTargetKey.SelectedKeyName = "AttackTarget";
    DistanceToTargetKey.SelectedKeyName = "DistanceToTarget";
}

void UBTS_UpdateDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
    {
        JHLOG_S(Error);
        return;
    }

    AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey.SelectedKeyName));
    if (!AttackTarget)
    {
        JHLOG_S(Error);
        return;
    }

    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        JHLOG_S(Error);
        return;
    }

    APawn* AIPawn = AIController->GetPawn();
    if (!AIPawn)
    {
        JHLOG_S(Error);
        return;
    }

    FVector TargetLocation = AttackTarget->GetActorLocation();
    FVector CurrentLocation = AIPawn->GetActorLocation();
    float Distance = FVector::Dist(CurrentLocation, TargetLocation);

    BlackboardComp->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, Distance);
}
