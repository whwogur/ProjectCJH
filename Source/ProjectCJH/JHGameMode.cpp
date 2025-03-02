// Fill out your copyright notice in the Description page of Project Settings.


#include "JHGameMode.h"
#include "JHPlayerController.h"
#include "JHCharacter.h"

AJHGameMode::AJHGameMode()
{
	DefaultPawnClass = AJHCharacter::StaticClass();
	PlayerControllerClass = AJHPlayerController::StaticClass();
}

void AJHGameMode::PostLogin(APlayerController* NewPlayer)
{
	JHLOG(Warning, TEXT("PostLogin"));
	Super::PostLogin(NewPlayer);
}
