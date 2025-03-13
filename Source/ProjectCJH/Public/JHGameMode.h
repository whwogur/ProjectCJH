// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/GameModeBase.h"
#include "JHGameMode.generated.h"

UCLASS()
class PROJECTCJH_API AJHGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AJHGameMode();

public:
	virtual void PostInitializeComponents() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	
public:
	void AddScore(class AJHPlayerController* PlayerController);

private:
	UPROPERTY()
	class AJHGameState* JHGameState;
};
