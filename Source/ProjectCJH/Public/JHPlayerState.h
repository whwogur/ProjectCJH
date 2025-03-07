// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/PlayerState.h"
#include "JHPlayerState.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateChangedDelegate);

UCLASS()
class PROJECTCJH_API AJHPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AJHPlayerState();

	int32 GetGameScore() const { return GameScore; }
	int32 GetCharacterLevel() const { return CharacterLevel; }
	int32 GetPotionCount() const { return Potions; }

	void InitPlayerData();

public:
	FOnPlayerStateChangedDelegate OnPlayerStateChanged;

protected:
	UPROPERTY(Transient)
	int32 GameScore;

	UPROPERTY(Transient)
	int32 CharacterLevel;

	UPROPERTY(Transient)
	int32 Potions;
};
