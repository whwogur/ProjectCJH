// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_AttackTarget.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UEnvQueryContext_AttackTarget::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
    // ������ �����ϴ� ����(���� ��) ��������
    AActor* QuerierActor = Cast<AActor>(QueryInstance.Owner.Get());
    if (!QuerierActor)
    {
        JHLOG_S(Error);
        return;
    }

    // ���� ������ AI ��Ʈ�ѷ� ��������
    AAIController* AIController = Cast<AAIController>(QuerierActor->GetInstigatorController());
    if (!AIController)
    {
        JHLOG_S(Error);
        return;
    }

    // �����忡�� Ÿ�� ��������
    UBlackboardComponent* BlackboardComp = AIController->GetBlackboardComponent();
    if (BlackboardComp)
    {
        AActor* AttackTarget = Cast<AActor>(BlackboardComp->GetValueAsObject("AttackTarget"));
        if (AttackTarget)
        {
            // Ÿ���� ���ؽ�Ʈ �����Ϳ� ����
            UEnvQueryItemType_Actor::SetContextHelper(ContextData, AttackTarget);
        }
    }
}