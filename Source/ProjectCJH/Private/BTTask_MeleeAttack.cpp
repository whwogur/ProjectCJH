// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MeleeAttack.h"

UBTTask_MeleeAttack::UBTTask_MeleeAttack()
{
    NodeName = "Melee Attack";
    bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    
    return EBTNodeResult::InProgress; // �񵿱� �½�ũ�� ���� �� ���� ��ȯ
}