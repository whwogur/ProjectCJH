// Fill out your copyright notice in the Description page of Project Settings.


#include "JHAnimInstance.h"

UJHAnimInstance::UJHAnimInstance()
	: CurrentPawnSpeed(0.0f)
	, IsInAir(false)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Animations/Combat/AttackCombo01.AttackCombo01"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}

}

void UJHAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* pOwner = TryGetPawnOwner();
	if (::IsValid(pOwner))
	{
		FVector Velocity = pOwner->GetVelocity();
		CurrentPawnSpeed = Velocity.Size();
		ACharacter* Character = Cast<ACharacter>(pOwner);
		if (Character)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
			FRotator Rotation = Character->GetActorRotation();
			CurrentPawnDirection = CalculateDirection(Velocity, Rotation);
		}
	}
}

void UJHAnimInstance::PlayAttackMontage()
{	
	Montage_Play(AttackMontage, 1.0f);
}
