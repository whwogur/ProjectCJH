// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemy_Melee.h"
#include "JHWeapon.h"
#include "JHCombatComponent.h"
#include "JHAnimInstance.h"

const FName AJHEnemy_Melee::WeaponSocketName_Equipped(TEXT("hand_rSocket"));
const FName AJHEnemy_Melee::WeaponSocketName_Sheathed(TEXT("weapon_pelvisSocket"));

AJHEnemy_Melee::AJHEnemy_Melee()
{
	WeaponClass = AJHWeapon::StaticClass();
}

void AJHEnemy_Melee::EquipWeapon()
{
    UJHAnimInstance* AnimInstance = Cast<UJHAnimInstance>(GetMesh()->GetAnimInstance());
    JHCHECK(AnimInstance);
    if (AnimInstance)
    {
        AnimInstance->Montage_Play(EquipWeaponMontage);
        FOnMontageEnded MontageEndedDelegate;
        MontageEndedDelegate.BindUObject(this, &AJHEnemyBase::OnWeaponEquipCompleted);
        AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, EquipWeaponMontage);
    }
}

void AJHEnemy_Melee::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    
}

void AJHEnemy_Melee::BeginPlay()
{
    Super::BeginPlay();
    
}

void AJHEnemy_Melee::OnAssetLoadCompleted()
{
    Super::OnAssetLoadCompleted();
    if (WeaponClass)
    {
        AJHWeapon* Weapon = GetWorld()->SpawnActor<AJHWeapon>(WeaponClass, FVector::ZeroVector, FRotator::ZeroRotator);
        if (Weapon)
        {
            SetWeapon(Weapon, WeaponSocketName_Sheathed);
        }
    }

    UJHAnimInstance* AnimInstance = Cast<UJHAnimInstance>(GetMesh()->GetAnimInstance());
    JHCHECK(AnimInstance);

    if (AnimInstance)
    {

        AnimInstance->OnPullOut.AddLambda([this]()
            {
                MoveWeapon(WeaponSocketName_Equipped);
            });
        AnimInstance->OnSheathe.AddLambda([this]()
            {
                MoveWeapon(WeaponSocketName_Sheathed);
            });
    }
}

void AJHEnemy_Melee::Attack()
{
    UJHAnimInstance* JHAnimInstance = StaticCast<UJHAnimInstance*>(GetMesh()->GetAnimInstance());
    JHAnimInstance->Montage_Play(AttackMontage);
    Combat->SetAttacking(true);
}