// Fill out your copyright notice in the Description page of Project Settings.


#include "JHAnimInstance.h"
#include "ComboCheckAnimNotify.h"
#include "KismetAnimationLibrary.h"

UJHAnimInstance::UJHAnimInstance()
	: CurrentPawnSpeed(0.0f)
	, IsInAir(false)
	, IsDead(false)
{
	
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



void UJHAnimInstance::AnimNotify_ApplyDamage()
{
	OnApplyDamage.Broadcast();
}

void UJHAnimInstance::AnimNotify_PullOut()
{
	OnPullOut.Broadcast();
}

void UJHAnimInstance::AnimNotify_Sheathe()
{
	OnSheathe.Broadcast();
}

void UJHAnimInstance::AnimNotify_ApplyAOEDamage()
{
	OnApplyAOEDamage.Broadcast();
}