// Fill out your copyright notice in the Description page of Project Settings.


#include "JHAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EMovementSpeed.h"
#include "EEnemyState.h"

const FName AJHAIController::AttackTargetKey(TEXT("AttackTarget"));
const FName AJHAIController::PointOfInterestKey(TEXT("PointOfInterest"));
const FName AJHAIController::AttackRadiusKey(TEXT("AttackRadius"));
const FName AJHAIController::DefendRadiusKey(TEXT("DefendRadius"));
const FName AJHAIController::DistanceToTargetKey(TEXT("DistanceToTarget"));
const FName AJHAIController::EnemyStateKey(TEXT("EnemyState"));

AJHAIController::AJHAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/AI/BB_EnemyBase.BB_EnemyBase"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/AI/BT_EnemyMelee.BT_EnemyMelee"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void AJHAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	Blackboard->SetValueAsEnum(EnemyStateKey, static_cast<uint8>(EEnemyState::PASSIVE));

	RunAI();
}

void AJHAIController::RunAI()
{
	UBlackboardComponent* BlackboardComp{};
	if (UseBlackboard(BBAsset, BlackboardComp))
	{
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

void AJHAIController::SetAIState(EEnemyState eState)
{
	Blackboard->SetValueAsEnum(EnemyStateKey, static_cast<uint8>(eState));
}

void AJHAIController::SetMovementSpeed(EMovementSpeed eMovementSpeed)
{
	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	if (ControlledCharacter)
	{
		UCharacterMovementComponent* MovementComp = ControlledCharacter->GetCharacterMovement();
		if (MovementComp)
		{
			switch (eMovementSpeed)
			{
			case EMovementSpeed::IDLE:
			{
				MovementComp->MaxWalkSpeed = 0.0f;
				break;
			}
			case EMovementSpeed::WALK:
			{
				MovementComp->MaxWalkSpeed = 150.0f;
				break;
			}
			case EMovementSpeed::JOG:
			{
				MovementComp->MaxWalkSpeed = 250.0f;
				break;
			}
			case EMovementSpeed::SPRINT:
			{
				MovementComp->MaxWalkSpeed = 500.0f;
				break;
			}
			}
		}
	}
}
