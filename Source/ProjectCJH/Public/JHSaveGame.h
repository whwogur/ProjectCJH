// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/SaveGame.h"
#include "JHSaveGame.generated.h"

UCLASS()
class PROJECTCJH_API UJHSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UJHSaveGame();

	UPROPERTY()
	int32 Level;

	UPROPERTY()
	int32 Exp;

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 Score;
};
