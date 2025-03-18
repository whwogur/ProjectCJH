// Fill out your copyright notice in the Description page of Project Settings.


#include "JHAnimInstance_Player.h"


UJHAnimInstance_Player::UJHAnimInstance_Player()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Animations/Combat/AttackCombo02_RM.AttackCombo02_RM"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}
void UJHAnimInstance_Player::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);
}

void UJHAnimInstance_Player::JumpToAttackMontageSection(int32 NewSection)
{
	JHCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

FName UJHAnimInstance_Player::GetAttackMontageSectionName(int32 Section)
{
	JHCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
