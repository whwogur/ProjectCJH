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
    JHCHECK(EnemyAnimInstance);
    if (EnemyAnimInstance)
    {
        EnemyAnimInstance->Montage_Play(EquipWeaponMontage);
        FOnMontageEnded MontageEndedDelegate;
        MontageEndedDelegate.BindUObject(this, &AJHEnemyBase::OnWeaponEquipCompleted);
        EnemyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, EquipWeaponMontage);
    }
}

void AJHEnemy_Melee::SheatheWeapon()
{
    JHCHECK(EnemyAnimInstance);
    if (EnemyAnimInstance)
    {
        EnemyAnimInstance->Montage_Play(SheatheWeaponMontage);
        FOnMontageEnded MontageEndedDelegate;
        MontageEndedDelegate.BindUObject(this, &AJHEnemyBase::OnWeaponSheatheCompleted);
        EnemyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, SheatheWeaponMontage);
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

    JHCHECK(EnemyAnimInstance);
    if (EnemyAnimInstance)
    {
        EnemyAnimInstance->OnPullOut.AddLambda([this]()
            {
                MoveWeapon(WeaponSocketName_Equipped);
            });
        EnemyAnimInstance->OnSheathe.AddLambda([this]()
            {
                MoveWeapon(WeaponSocketName_Sheathed);
            });

        FOnMontageEnded MontageEndedDelegate;
        MontageEndedDelegate.BindUObject(this, &AJHCharacter::OnAttackMontageEnded);
        EnemyAnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);
    }
}

void AJHEnemy_Melee::Attack()
{
    JHCHECK(EnemyAnimInstance);
    if (EnemyAnimInstance)
    {
        EnemyAnimInstance->Montage_Play(AttackMontage);
    }
    IsAttacking = true;
}