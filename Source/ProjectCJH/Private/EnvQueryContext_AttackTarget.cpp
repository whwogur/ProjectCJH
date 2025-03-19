// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_AttackTarget.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
const FName UEnvQueryContext_AttackTarget::AttackTargetKey(TEXT("AttackTarget"));

void UEnvQueryContext_AttackTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    // 쿼리를 실행하는 액터(보통 폰) 가져오기
    AActor* QuerierActor = Cast<AActor>(QueryInstance.Owner.Get());
    if (!QuerierActor)
    {
        JHLOG_S(Error);
        return;
    }

    // 쿼리 액터의 AI 컨트롤러 가져오기
    AAIController* AIController = Cast<AAIController>(QuerierActor->GetInstigatorController());
    if (!AIController)
    {
        JHLOG_S(Error);
        return;
    }

    // 블랙보드에서 타겟 가져오기
    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (BlackboardComp)
    {
        AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(AttackTargetKey));
        if (AttackTarget)
        {
            UEnvQueryItemType_Actor::SetContextHelper(ContextData, AttackTarget);
        }
    }
}