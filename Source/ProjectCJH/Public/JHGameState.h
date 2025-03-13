// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/GameStateBase.h"
#include "JHGameState.generated.h"

UCLASS()
class PROJECTCJH_API AJHGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AJHGameState();

public:
	int32 GetTotalGameScore() const { return TotalGameScore; }
	void AddGameScore() { ++TotalGameScore; }

private:
	UPROPERTY(Transient);
	int32 TotalGameScore;
};
