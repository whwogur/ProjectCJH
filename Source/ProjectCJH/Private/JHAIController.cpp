// Fill out your copyright notice in the Description page of Project Settings.


#include "JHAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AJHAIController::InitialPosKey(TEXT("InitialPos"));
const FName AJHAIController::PointOfInterestKey(TEXT("PointOfInterest"));
const FName AJHAIController::TargetKey(TEXT("Target"));

AJHAIController::AJHAIController()
	: RepeatInterval(3.0f)
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_JHCharacter.BB_JHCharacter"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_JHCharacter.BT_JHCharacter"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void AJHAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	RunAI();
}

void AJHAIController::OnUnPossess()
{
	Super::OnUnPossess();
	GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

void AJHAIController::RunAI()
{
	UBlackboardComponent* BlackboardComp{};
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		Blackboard->SetValueAsVector(InitialPosKey, GetPawn()->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			JHLOG(Error, TEXT("AIController couldn't run behavior tree"));
		}
	}
	else JHLOG_S(Warning);
}

void AJHAIController::StopAI()
{
	UBehaviorTreeComponent* BTComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr != BTComponent)
	{
		BTComponent->StopTree(EBTStopMode::Safe);
	}
}

void AJHAIController::OnRepeatTimer()
{
	APawn* CurrentPawn = GetPawn();
	JHCHECK(CurrentPawn);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (nullptr == NavSystem)
	{
		JHLOG_S(Warning);
		return;
	}

	FNavLocation NextLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
		JHLOG(Warning, TEXT("Next Location: %s"), *NextLocation.Location.ToString());
	}
}
