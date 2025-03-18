// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EquipWeapon.h"
#include "AIController.h"
#include "JHEnemyBase.h"

// ===============
// BTT EquipWeapon
// ===============
UBTTask_EquipWeapon::UBTTask_EquipWeapon()
{
	NodeName = "Equip Weapon";
}

EBTNodeResult::Type UBTTask_EquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    APawn* Pawn = AIController->GetPawn();
    if (!Pawn)
    {
        return EBTNodeResult::Failed;
    }

    AJHEnemyBase* Enemy = Cast<AJHEnemyBase>(Pawn);
    if (!Enemy)
    {
        return EBTNodeResult::Failed;
    }

    EnemyWeakPtr = Enemy;

    Enemy->EquipWeapon();

    TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompWeakPtr = &OwnerComp;
    DelegateHandle = Enemy->OnWeaponEquipped.AddLambda([this, OwnerCompWeakPtr]()
        {
            if (OwnerCompWeakPtr.IsValid() && EnemyWeakPtr.IsValid())
            {
                AJHEnemyBase* EnemyPtr = EnemyWeakPtr.Get();
                UBehaviorTreeComponent* OwnerCompPtr = OwnerCompWeakPtr.Get();
                if (EnemyPtr && OwnerCompPtr)
                {
                    EnemyPtr->OnWeaponEquipped.Remove(DelegateHandle);
                    FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
                }
            }
        });

    return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_EquipWeapon::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    if (EnemyWeakPtr.IsValid() && DelegateHandle.IsValid())
    {
        AJHEnemyBase* Enemy = EnemyWeakPtr.Get();
        if (Enemy)
        {
            Enemy->OnWeaponEquipped.Remove(DelegateHandle);
        }
    }

    return Super::AbortTask(OwnerComp, NodeMemory);
}

// ===============
// BTT SheatheWeapon
// ===============

UBTTask_SheatheWeapon::UBTTask_SheatheWeapon()
{
    NodeName = "Sheathe Weapon";
}

EBTNodeResult::Type UBTTask_SheatheWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    APawn* Pawn = AIController->GetPawn();
    if (!Pawn)
    {
        return EBTNodeResult::Failed;
    }

    AJHEnemyBase* Enemy = Cast<AJHEnemyBase>(Pawn);
    if (!Enemy)
    {
        return EBTNodeResult::Failed;
    }

    EnemyWeakPtr = Enemy;

    Enemy->SheatheWeapon();

    TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompWeakPtr = &OwnerComp;
    DelegateHandle = Enemy->OnWeaponSheathed.AddLambda([this, OwnerCompWeakPtr]()
        {
            if (OwnerCompWeakPtr.IsValid() && EnemyWeakPtr.IsValid())
            {
                AJHEnemyBase* EnemyPtr = EnemyWeakPtr.Get();
                UBehaviorTreeComponent* OwnerCompPtr = OwnerCompWeakPtr.Get();
                if (EnemyPtr && OwnerCompPtr)
                {
                    // ��������Ʈ ����
                    EnemyPtr->OnWeaponSheathed.Remove(DelegateHandle);
                    // �½�ũ �Ϸ�
                    FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
                }
            }
        });

    // �½�ũ�� ���� �� ���·� ��ȯ
    return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_SheatheWeapon::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Enemy�� DelegateHandle�� ��ȿ�� ��� ��������Ʈ ����
    if (EnemyWeakPtr.IsValid() && DelegateHandle.IsValid())
    {
        AJHEnemyBase* Enemy = EnemyWeakPtr.Get();
        if (Enemy)
        {
            Enemy->OnWeaponSheathed.Remove(DelegateHandle);
        }
    }

    // �θ� Ŭ������ AbortTask ȣ��
    return Super::AbortTask(OwnerComp, NodeMemory);
}
