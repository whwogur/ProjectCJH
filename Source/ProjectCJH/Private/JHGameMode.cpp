// Fill out your copyright notice in the Description page of Project Settings.


#include "JHGameMode.h"
#include "JHPlayerController.h"
#include "JHPlayerCharacter.h"
#include "JHPlayerState.h"
#include "JHGameState.h"

AJHGameMode::AJHGameMode()
{
	DefaultPawnClass = AJHPlayerCharacter::StaticClass();
	PlayerControllerClass = AJHPlayerController::StaticClass();
	PlayerStateClass = AJHPlayerState::StaticClass();
	GameStateClass = AJHGameState::StaticClass();
}

void AJHGameMode::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	JHGameState = Cast<AJHGameState>(GameState);
}

void AJHGameMode::PostLogin(APlayerController* NewPlayer)
{
	JHLOG(Warning, TEXT("PostLogin"));
	Super::PostLogin(NewPlayer);

	AJHPlayerState* JHPlayerState = Cast<AJHPlayerState>(NewPlayer->PlayerState);
	JHCHECK(JHPlayerState);
	JHPlayerState->InitPlayerData();
}

void AJHGameMode::AddScore(AJHPlayerController* PlayerController)
{
	for (FConstPlayerControllerIterator it = GetWorld()->GetPlayerControllerIterator(); it; ++it)
	{
		const AJHPlayerController* playerController = Cast<AJHPlayerController>(it->Get());
		if ((nullptr != playerController) && (PlayerController == playerController))
		{
			playerController->AddGameScore();
			break;
		}
	}

	JHGameState->AddGameScore();
}
