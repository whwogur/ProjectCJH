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
    // AI 컨트롤러 가져오기
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
    {
        return EBTNodeResult::Failed;
    }

    // 제어되는 폰 가져오기
    APawn* Pawn = AIController->GetPawn();
    if (!Pawn)
    {
        return EBTNodeResult::Failed;
    }

    // 폰을 ABP_Enemy_Base_C로 캐스팅
    AJHEnemyBase* Enemy = Cast<AJHEnemyBase>(Pawn);
    if (!Enemy)
    {
        return EBTNodeResult::Failed;
    }

    // 약한 포인터에 적 객체 저장
    EnemyWeakPtr = Enemy;

    // 무기 장착 함수 호출
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
                    // 델리게이트 제거
                    EnemyPtr->OnWeaponEquipped.Remove(DelegateHandle);
                    // 태스크 완료
                    FinishLatentTask(*OwnerCompPtr, EBTNodeResult::Succeeded);
                }
            }
        });

    // 태스크를 진행 중 상태로 반환
    return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_EquipWeapon::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Enemy와 DelegateHandle이 유효한 경우 델리게이트 제거
    if (EnemyWeakPtr.IsValid() && DelegateHandle.IsValid())
    {
        AJHEnemyBase* Enemy = EnemyWeakPtr.Get();
        if (Enemy)
        {
            Enemy->OnWeaponEquipped.Remove(DelegateHandle);
        }
    }

    // 부모 클래스의 AbortTask 호출
    return Super::AbortTask(OwnerComp, NodeMemory);
}
