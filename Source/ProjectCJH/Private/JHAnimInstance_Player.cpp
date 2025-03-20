// Fill out your copyright notice in the Description page of Project Settings.


#include "JHAnimInstance_Player.h"


UJHAnimInstance_Player::UJHAnimInstance_Player()
{
	
}
void UJHAnimInstance_Player::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

void UJHAnimInstance_Player::JumpToAttackMontageSection(int32 NewSection, const UAnimMontage* Montage)
{
	JHCHECK(Montage_IsPlaying(Montage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), Montage);
}

FName UJHAnimInstance_Player::GetAttackMontageSectionName(int32 Section)
{
	JHCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
