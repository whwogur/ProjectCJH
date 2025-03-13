// Fill out your copyright notice in the Description page of Project Settings.


#include "JHPlayerState.h"
#include "JHGameInstance.h"
#include "JHSaveGame.h"

AJHPlayerState::AJHPlayerState()
	: SaveSlotName(TEXT("Slot1"))
	, GameScore(0)
	, CharacterLevel(1)
	, Potions(0)
	, Exp(0)
	, CurrentStatData(nullptr)
{
}

void AJHPlayerState::AddGameScore()
{
	++GameScore;
	OnPlayerStateChanged.Broadcast();
	SavePlayerData();
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
	SavePlayerData();
	return DidLevelUp;
}

void AJHPlayerState::InitPlayerData()
{
	UJHSaveGame* savedGame = Cast<UJHSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (nullptr == savedGame)
	{
		savedGame = GetMutableDefault<UJHSaveGame>();
	}


	SetPlayerName(savedGame->PlayerName);
	SetCharacterLevel(savedGame->Level);
	GameScore = savedGame->Score;
	Potions = 5;
	Exp = savedGame->Exp;

	SavePlayerData();
}

void AJHPlayerState::SavePlayerData()
{
	UJHSaveGame* newPlayerData = NewObject<UJHSaveGame>();
	newPlayerData->PlayerName = GetPlayerName();
	newPlayerData->Level = CharacterLevel;
	newPlayerData->Exp = Exp;
	newPlayerData->Score = GameScore;

	if (UGameplayStatics::SaveGameToSlot(newPlayerData, SaveSlotName, 0))
	{
		JHLOG(Error, TEXT("Failed To Save Game"));
	}
}

void AJHPlayerState::SetCharacterLevel(int32 NewCharacterLevel)
{
	UJHGameInstance* JHGameInstance = Cast<UJHGameInstance>(GetGameInstance());
	JHCHECK(JHGameInstance);

	CurrentStatData = JHGameInstance->GetJHCharacterData(NewCharacterLevel);
	JHCHECK(CurrentStatData);

	CharacterLevel = NewCharacterLevel;
}
