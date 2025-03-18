// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EquipWeapon.h"
#include "AIController.h"
#include "JHEnemyBase.h"
UBTTask_EquipWeapon::UBTTask_EquipWeapon()
{
	NodeName = "Equip Weapon";
}

EBTNodeResult::Type UBTTask_EquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // AI ��Ʈ�ѷ� ��������
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    // ����Ǵ� �� ��������
    APawn* Pawn = AIController->GetPawn();
    if (!Pawn)
    {
        return EBTNodeResult::Failed;
    }

    // ���� ABP_Enemy_Base_C�� ĳ����
    AJHEnemyBase* Enemy = Cast<AJHEnemyBase>(Pawn);
    if (!Enemy)
    {
        return EBTNodeResult::Failed;
    }

    // ���� �����Ϳ� �� ��ü ����
    EnemyWeakPtr = Enemy;

    // ���� ���� �Լ� ȣ��
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
                    // ��������Ʈ ����
                    EnemyPtr->OnWeaponEquipped.Remove(DelegateHandle);
                    // �½�ũ �Ϸ�
                    FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
                }
            }
        });

    // �½�ũ�� ���� �� ���·� ��ȯ
    return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_EquipWeapon::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Enemy�� DelegateHandle�� ��ȿ�� ��� ��������Ʈ ����
    if (EnemyWeakPtr.IsValid() && DelegateHandle.IsValid())
    {
        AJHEnemyBase* Enemy = EnemyWeakPtr.Get();
        if (Enemy)
        {
            Enemy->OnWeaponEquipped.Remove(DelegateHandle);
        }
    }

    // �θ� Ŭ������ AbortTask ȣ��
    return Super::AbortTask(OwnerComp, NodeMemory);
}
