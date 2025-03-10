// Fill out your copyright notice in the Description page of Project Settings.


#include "JHCombatComponent.h"
#include "Engine/DamageEvents.h"
#include "JHWeapon.h"
// Sets default values for this component's properties
UJHCombatComponent::UJHCombatComponent()
    : CurrentWeapon(nullptr)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

void UJHCombatComponent::ApplyDamage(const FAttackInfo& AttackInfo)
{
    FHitResult HitResult;
    FCollisionQueryParams Params(NAME_None, false, GetOwner());
    FVector aLocation = GetOwner()->GetActorLocation();
    FVector aForwardVector = GetOwner()->GetActorForwardVector();
    bool bResult = GetWorld()->SweepSingleByChannel(
        HitResult,
        aLocation,
        aLocation + aForwardVector * 200.0f,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel2,
        FCollisionShape::MakeSphere(50.0f),
        Params
    );

#if ENABLE_DRAW_DEBUG
    FVector TraceVec = aForwardVector * AttackInfo.Range;
    FVector Center = aLocation + TraceVec * 0.5f;
    float HalfHeight = (CurrentWeapon == nullptr ? (AttackInfo.Range * 0.5f) : CurrentWeapon->GetAttackRange()) + AttackInfo.Radius;
    FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
    FColor DrawColor = bResult ? FColor::Red : FColor::Green;
    float DebugLifeTime = 2.0f;

    DrawDebugCapsule(GetWorld(),
        Center,
        HalfHeight,
        AttackInfo.Radius,
        CapsuleRot,
        DrawColor,
        false,
        DebugLifeTime);
#endif

    if (bResult)
    {
        FDamageEvent DamageEvent;
        HitResult.GetActor()->TakeDamage(AttackInfo.Damage, DamageEvent, GetOwner()->GetInstigatorController(), GetOwner());
    }
}
// 현재 장착 무기 있는지 여부
bool UJHCombatComponent::CanSetWeapon()
{
    return (nullptr == CurrentWeapon);
}

void UJHCombatComponent::SetWeapon(AJHWeapon* NewWeapon)
{
    JHCHECK((nullptr != NewWeapon && nullptr == CurrentWeapon));
}

void UJHCombatComponent::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    JHCHECK(bAttacking);
    bAttacking = false;
    OnAttackEnd.Broadcast();
}
