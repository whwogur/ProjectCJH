// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_DetectTarget.h"
#include "JHAIController.h"
#include "JHCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"

UBTService_DetectTarget::UBTService_DetectTarget()
{
	NodeName = TEXT("DetectTarget");
	Interval = 1.0f;
}

void UBTService_DetectTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 600.0f;

	if (nullptr == World)
		return;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (const FOverlapResult& OverlapResult : OverlapResults)
		{
			AJHCharacter* Character = Cast<AJHCharacter>(OverlapResult.GetActor());
			if (Character && Character->GetController()->IsPlayerController())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AJHAIController::TargetKey, Character);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Character->GetActorLocation(), FColor::Red, false, 0.2f);
				return;
			}
		}
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AJHAIController::TargetKey, nullptr);
	}

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Emerald, false, 0.2f);
}
