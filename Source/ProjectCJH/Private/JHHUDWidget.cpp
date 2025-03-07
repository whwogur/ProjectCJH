// Fill out your copyright notice in the Description page of Project Settings.


#include "JHHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "JHCharacterStatComponent.h"
#include "JHPlayerState.h"

void UJHHUDWidget::BindCharacterStat(UJHCharacterStatComponent* CharacterStat)
{
	JHCHECK(CharacterStat);
	CurrentCharacterStat = CharacterStat;
	CharacterStat->OnHPChanged.AddUObject(this, &UJHHUDWidget::UpdateCharacterStat);
}

void UJHHUDWidget::BindPlayerState(AJHPlayerState* PlayerState)
{
	JHCHECK(PlayerState);
	CurrentPlayerState = PlayerState;
	PlayerState->OnPlayerStateChanged.AddUObject(this, &UJHHUDWidget::UpdatePlayerState);
}

void UJHHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbHP")));
	JHCHECK(HPBar);

	ExpBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("pbEXP")));
	JHCHECK(ExpBar);

	PlayerName = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPlayerName")));
	JHCHECK(PlayerName);

	PlayerLevel = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtLevel")));
	JHCHECK(PlayerLevel);

	CurrentScore = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtCurrentScore")));
	JHCHECK(CurrentScore);

	PotionCount = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtPotionCount")));
	JHCHECK(PotionCount);
}

void UJHHUDWidget::UpdateCharacterStat()
{
	JHCHECK(CurrentCharacterStat.IsValid());
	HPBar->SetPercent(CurrentCharacterStat->GetHPRatio());
}

void UJHHUDWidget::UpdatePlayerState()
{
	JHCHECK(CurrentPlayerState.IsValid());

	PlayerName->SetText(FText::FromString(CurrentPlayerState->GetPlayerName()));
	PlayerLevel->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetCharacterLevel())));
	CurrentScore->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetGameScore())));
	PotionCount->SetText(FText::FromString(FString::FromInt(CurrentPlayerState->GetPotionCount())));
	ExpBar->SetPercent(CurrentPlayerState->GetExpRatio());
}
