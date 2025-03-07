// Fill out your copyright notice in the Description page of Project Settings.


#include "JHPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "JHHUDWidget.h"
#include "JHPlayerState.h"

AJHPlayerController::AJHPlayerController()
{
	static ConstructorHelpers::FClassFinder<UJHHUDWidget> UI_HUD_C(TEXT("/Game/UI/UI_HUD.UI_HUD_C"));
	if (UI_HUD_C.Succeeded())
	{
		HUDWidgetClass = UI_HUD_C.Class;
	}
}

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

	HUDWidget = CreateWidget<UJHHUDWidget>(this, HUDWidgetClass);
	HUDWidget->AddToViewport();

	AJHPlayerState* pState = Cast<AJHPlayerState>(PlayerState);
	JHCHECK(pState);
	HUDWidget->BindPlayerState(pState);
	pState->OnPlayerStateChanged.Broadcast();
}

UJHHUDWidget* AJHPlayerController::GetHUDWidget() const
{
	return HUDWidget;
}
