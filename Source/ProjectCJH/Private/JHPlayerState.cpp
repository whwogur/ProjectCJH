// Fill out your copyright notice in the Description page of Project Settings.


#include "JHPlayerState.h"

AJHPlayerState::AJHPlayerState()
	: CharacterLevel(1)
	, GameScore(0)
	, Potions(0)
{
}

void AJHPlayerState::InitPlayerData()
{
	SetPlayerName(TEXT("Wolf"));
	CharacterLevel = 5;
	GameScore = 0;
	Potions = 5;
}
