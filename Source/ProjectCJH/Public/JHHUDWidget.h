// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Blueprint/UserWidget.h"
#include "JHHUDWidget.generated.h"

class UJHCharacterStatComponent;
class AJHPlayerState;
class UProgressBar;
class UTextBlock;

UCLASS()
class PROJECTCJH_API UJHHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindCharacterStat(UJHCharacterStatComponent* CharacterStat);
	void BindPlayerState(AJHPlayerState* PlayerState);

protected:
	virtual void NativeConstruct() override;
	void UpdateCharacterStat();
	void UpdatePlayerState();

private:
	TWeakObjectPtr<UJHCharacterStatComponent> CurrentCharacterStat;
	TWeakObjectPtr<AJHPlayerState> CurrentPlayerState;

	UPROPERTY()
	UProgressBar* HPBar;

	UPROPERTY()
	UProgressBar* ExpBar;

	UPROPERTY()
	UTextBlock* PlayerName;

	UPROPERTY()
	UTextBlock* PlayerLevel;

	UPROPERTY()
	UTextBlock* CurrentScore;

	UPROPERTY()
	UTextBlock* PotionCount;
};
