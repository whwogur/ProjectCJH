// Fill out your copyright notice in the Description page of Project Settings.


#include "JHCharacterWidget.h"
#include "JHCharacterStatComponent.h"

void UJHCharacterWidget::BindCharacterStat(UJHCharacterStatComponent* NewCharacterStat)
{
	JHCHECK(NewCharacterStat);

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddLambda([this]() ->void {
		if (CurrentCharacterStat.IsValid())
		{
			JHLOG(Warning, TEXT("HPRatio: %f"), CurrentCharacterStat->GetHPRatio());
		}
	});
}
