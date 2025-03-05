// Fill out your copyright notice in the Description page of Project Settings.


#include "JHPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

void AJHPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	JHLOG_S(Warning);
}

void AJHPlayerController::BeginPlay()
{
	Super::BeginPlay();
    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
}