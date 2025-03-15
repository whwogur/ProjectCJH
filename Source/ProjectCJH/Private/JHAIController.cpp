// Fill out your copyright notice in the Description page of Project Settings.


#include "JHAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Kismet/KismetSystemLibrary.h"
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

	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	JHCHECK(nullptr != Blackboard);
	
	// 퍼셉션 컴포넌트 생성
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	JHCHECK(nullptr != AIPerception)
	// 시각 감각 설정
	UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 500.0f;
	SightConfig->LoseSightRadius = 600.0f;
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	AIPerception->ConfigureSense(*SightConfig);

	// 청각 감각 설정
	UAISenseConfig_Hearing* HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	HearingConfig->HearingRange = 300.0f;
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	AIPerception->ConfigureSense(*HearingConfig);
}

void AJHAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	JHCHECK(InitializeBlackboard(*Blackboard, *BBAsset));

	Blackboard->SetValueAsEnum(EnemyStateKey, static_cast<uint8>(EEnemyState::PASSIVE));

	// 퍼셉션 업데이트 이벤트 바인딩
	if (AIPerception)
	{
		AIPerception->OnPerceptionUpdated.AddDynamic(this, &AJHAIController::HandlePerceptionUpdate);
	}

	// 주기적 확인 타이머 설정 (2초마다 실행)
	GetWorld()->GetTimerManager().SetTimer(
		CheckForgottenTargetTimer,
		this,
		&AJHAIController::CheckIfForgottenSeenActor,
		3.0f,
		true
	);

	JHCHECK(RunBehaviorTree(BTAsset));
}

void AJHAIController::OnUnPossess()
{
	Super::OnUnPossess();

	// 타이머 정리
	GetWorld()->GetTimerManager().ClearTimer(CheckForgottenTargetTimer);
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

bool AJHAIController::CanSenseActor(AActor* Actor, EAISense Sense, FAIStimulus& OutStimulus)
{
	if (!AIPerception || !Actor)
	{
		return false;
	}
	JHLOG_SIMPLE(TEXT("Can sense %s"), *Actor->GetName());
	// 감각 설정 가져오기
	UAISenseConfig* SenseConfig = nullptr;
	switch (Sense)
	{
	case EAISense::Sight:
		SenseConfig = AIPerception->GetSenseConfig<UAISenseConfig_Sight>();
		break;
	case EAISense::Hearing:
		SenseConfig = AIPerception->GetSenseConfig<UAISenseConfig_Hearing>();
	}

	if (!SenseConfig)
	{
		return false;
	}

	// 액터 감지 여부 확인
	FActorPerceptionBlueprintInfo PerceptionInfo;
	if (AIPerception->GetActorsPerception(Actor, PerceptionInfo))
	{
		for (const FAIStimulus& Stim : PerceptionInfo.LastSensedStimuli)
		{
			if (Stim.Type == SenseConfig->GetSenseID() && Stim.WasSuccessfullySensed())
			{
				OutStimulus = Stim;
				return true;
			}
		}
	}

	return false;
}

void AJHAIController::HandlePerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* Actor : UpdatedActors)
	{
		FAIStimulus Stimulus;
		if (CanSenseActor(Actor, EAISense::Sight, Stimulus))
		{
			Blackboard->SetValueAsObject(AttackTargetKey, Actor);
			SetAIState(EEnemyState::SEEK);
		}
		else if (CanSenseActor(Actor, EAISense::Hearing, Stimulus))
		{
			Blackboard->SetValueAsVector(PointOfInterestKey, Actor->GetActorLocation());
			SetAIState(EEnemyState::INVESTIGATE);
		}
	}
}

void AJHAIController::CheckIfForgottenSeenActor()
{
	UObject* pAttackTarget = Blackboard->GetValueAsObject(AttackTargetKey);

	if (pAttackTarget)
	{
		FAIStimulus Stimulus;
		AActor* pActorTarget = Cast<AActor>(pAttackTarget);

		if (!CanSenseActor(pActorTarget, EAISense::Sight, Stimulus))
		{
			Blackboard->SetValueAsObject(AttackTargetKey, nullptr);
			SetAIState(EEnemyState::PASSIVE);
		}
	}
}
