// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FaceTarget.h"
#include "JHAIController.h"
#include "JHCharacter.h"
#include "BehaviorTree\BlackboardComponent.h"

UBTTask_FaceTarget::UBTTask_FaceTarget()
{
	NodeName = TEXT("FaceTarget");
}

EBTNodeResult::Type UBTTask_FaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AJHCharacter* Character = Cast<AJHCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == Character)
		return EBTNodeResult::Failed;

	AJHCharacter* AttackTarget = Cast<AJHCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AJHAIController::TargetKey));
	if (nullptr == AttackTarget)
		return EBTNodeResult::Failed;

	FVector LookVector = AttackTarget->GetActorLocation() - Character->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	Character->SetActorRotation(FMath::RInterpTo(Character->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}
