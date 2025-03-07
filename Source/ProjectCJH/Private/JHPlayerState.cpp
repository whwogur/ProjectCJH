// Fill out your copyright notice in the Description page of Project Settings.


#include "JHPlayerState.h"
#include "JHGameInstance.h"

AJHPlayerState::AJHPlayerState()
	: CharacterLevel(1)
	, GameScore(0)
	, Potions(0)
	, Exp(0)
	, CurrentStatData(nullptr)
{
}

float AJHPlayerState::GetExpRatio() const
{
	if (nullptr == CurrentStatData)
	{
		JHLOG(Error, TEXT("No Stat Data"));
		return 0.0f;
	}

	if(CurrentStatData->NextExp <= KINDA_SMALL_NUMBER)
	{
		JHLOG(Error, TEXT("Next Exp == 0"));
		return 0.0f;
	}

	float Result = (float)Exp / (float)CurrentStatData->NextExp;
	JHLOG(Warning, TEXT("Ratio: %f, Current: %d, Next: %d"), Result, Exp, CurrentStatData->NextExp);
	return Result;
}

bool AJHPlayerState::AddExp(int32 AccExp)
{
	if (CurrentStatData->NextExp == -1)
		return false;

	bool DidLevelUp = false;
	Exp = Exp + AccExp;
	if (Exp >= CurrentStatData->NextExp)
	{
		Exp -= CurrentStatData->NextExp;
		SetCharacterLevel(CharacterLevel + 1);
		DidLevelUp = true;
	}

	OnPlayerStateChanged.Broadcast();

	return DidLevelUp;
}

void AJHPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Wolf"));
	SetCharacterLevel(1);
	GameScore = 0;
	Potions = 5;
	Exp = 0;
}

void AJHPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	UJHGameInstance* JHGameInstance = Cast<UJHGameInstance>(GetGameInstance());
	JHCHECK(JHGameInstance);

	CurrentStatData = JHGameInstance->GetJHCharacterData(NewCharacterLevel);
	JHCHECK(CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}
