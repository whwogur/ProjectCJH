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
                    // 델리게이트 제거
                    EnemyPtr->OnWeaponSheathed.Remove(DelegateHandle);
                    // 태스크 완료
                    FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
                }
            }
        });

    // 태스크를 진행 중 상태로 반환
    return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_SheatheWeapon::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Enemy와 DelegateHandle이 유효한 경우 델리게이트 제거
    if (EnemyWeakPtr.IsValid() && DelegateHandle.IsValid())
    {
        AJHEnemyBase* Enemy = EnemyWeakPtr.Get();
        if (Enemy)
        {
            Enemy->OnWeaponSheathed.Remove(DelegateHandle);
        }
    }

    // 부모 클래스의 AbortTask 호출
    return Super::AbortTask(OwnerComp, NodeMemory);
}
