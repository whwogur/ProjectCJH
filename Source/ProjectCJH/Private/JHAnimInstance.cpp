// Fill out your copyright notice in the Description page of Project Settings.


#include "JHAnimInstance.h"
#include "ComboCheckAnimNotify.h"
#include "KismetAnimationLibrary.h"

UJHAnimInstance::UJHAnimInstance()
	: CurrentPawnSpeed(0.0f)
	, IsInAir(false)
	, IsDead(false)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Animations/Combat/AttackCombo02_RM.AttackCombo02_RM"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UJHAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* pOwner = TryGetPawnOwner();
	if (::IsValid(pOwner) && !IsDead)
	{
		FVector Velocity = pOwner->GetVelocity();
		CurrentPawnSpeed = Velocity.Size();
		ACharacter* Character = Cast<ACharacter>(pOwner);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
			FRotator Rotation = Character->GetActorRotation();
			CurrentPawnDirection = UKismetAnimationLibrary::CalculateDirection(Velocity, Rotation);
		}
	}
}

void UJHAnimInstance::PlayAttackMontage()
{	
	Montage_Play(AttackMontage, 1.0f);
}

void UJHAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	JHCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UJHAnimInstance::AnimNotify_ApplyDamage()
{
	OnApplyDamage.Broadcast();
}

void UJHAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UJHAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	JHCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 4), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
