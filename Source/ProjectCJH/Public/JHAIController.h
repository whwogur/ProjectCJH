// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "AIController.h"
#include "JHAIController.generated.h"

enum class EMovementSpeed : uint8;
enum class EEnemyState : uint8;

UENUM(BlueprintType)
enum class EAISense : uint8
{
	Sight,
	Hearing,
};

UCLASS()
class PROJECTCJH_API AJHAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AJHAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	static const FName AttackTargetKey;
	static const FName PointOfInterestKey;
	static const FName AttackRadiusKey;
	static const FName DefendRadiusKey;
	static const FName DistanceToTargetKey;
	static const FName EnemyStateKey;

	void StopAI();

	void SetAIState(EEnemyState eState);
	void SetMovementSpeed(EMovementSpeed eMovementSpeed);

public:
	UFUNCTION(BlueprintCallable)
	bool CanSenseActor(AActor* Actor, EAISense Sense, FAIStimulus& OutStimulus);

	UFUNCTION()
	void HandlePerceptionUpdate(const TArray<AActor*>& UpdatedActors);

private:
	// 잊혀진 액터 확인 함수
	void CheckIfForgottenSeenActor();
	void HandleSightUpdate(AActor* Actor);
	void HandleHearingUpdate(const FVector& SoundPos);
	// 주기적으로 잊혀진 타겟을 확인하기 위한 타이머 핸들
	FTimerHandle CheckForgottenTargetTimer;

private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;
	
	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	class UAIPerceptionComponent* AIPerception;
};
