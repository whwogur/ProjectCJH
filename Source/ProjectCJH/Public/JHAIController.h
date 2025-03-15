// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "AIController.h"
#include "JHAIController.generated.h"

enum class EMovementSpeed : uint8;
enum class EEnemyState : uint8;

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
	static const FName EnemyStateKey;

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
