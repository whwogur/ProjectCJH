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

	inline int32 GetGameScore() const { return GameScore; }
	inline int32 GetCharacterLevel() const { return CharacterLevel; }
	inline int32 GetPotionCount() const { return Potions; }
	float GetExpRatio() const;
	bool AddExp(int32 AccExp);
	inline void AddGameScore() { ++GameScore; OnPlayerStateChanged.Broadcast(); }
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

	UPROPERTY(Transient)
	int32 Exp;

private:
	void SetCharacterLevel(int32 NewCharacterLevel);
	struct FJHCharacterData* CurrentStatData;
};
