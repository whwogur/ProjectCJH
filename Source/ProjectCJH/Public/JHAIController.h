// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "AIController.h"
#include "EMovementSpeed.h"
#include "JHAIController.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	PASSIVE			UMETA(DisplayName = "Passive"),
	ATTACK			UMETA(DisplayName = "Attack"),
	STUN			UMETA(DisplayName = "Stun"),
	INVESTIGATE		UMETA(DisplayName = "Investigate"),
	DEAD			UMETA(DisplayName = "Dead"),
	SEEK			UMETA(DisplayName = "Seek"),
};

UCLASS()
class PROJECTCJH_API AJHAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AJHAIController();
	virtual void OnPossess(APawn* InPawn) override;
	
	static const FName AttackTargetKey;
	static const FName PointOfInterestKey;
	static const FName AttackRadiusKey;
	static const FName DefendRadiusKey;
	static const FName DistanceToTargetKey;
	static const FName AIStateKey;

	void RunAI();
	void StopAI();

	void SetAIState(EEnemyState eState);
	void SetMovementSpeed(EMovementSpeed eMovementSpeed);
private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;
	
	UPROPERTY()
	class UBlackboardData* BBAsset;
};
