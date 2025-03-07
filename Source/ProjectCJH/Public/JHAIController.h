// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "AIController.h"
#include "JHAIController.generated.h"

UCLASS()
class PROJECTCJH_API AJHAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AJHAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
	static const FName InitialPosKey;
	static const FName PointOfInterestKey;
	static const FName TargetKey;

	void RunAI();
	void StopAI();
private:
	UPROPERTY()
	class UBehaviorTree* BTAsset;
	
	UPROPERTY()
	class UBlackboardData* BBAsset;

private:
	void OnRepeatTimer();

	FTimerHandle	RepeatTimerHandle;
	float			RepeatInterval;
};
