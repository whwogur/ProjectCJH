// Fill out your copyright notice in the Description page of Project Settings.


#include "JHCharacterWidget.h"
#include "JHCharacterStatComponent.h"
#include "Components/ProgressBar.h"

void UJHCharacterWidget::BindCharacterStat(UJHCharacterStatComponent* NewCharacterStat)
{
	JHCHECK((nullptr != NewCharacterStat));

	CurrentCharacterStat = NewCharacterStat;
	NewCharacterStat->OnHPChanged.AddUObject(this, &UJHCharacterWidget::UpdateHPWidget);
}

void UJHCharacterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBar")));
	JHCHECK(HPProgressBar);
	UpdateHPWidget();
}

void UJHCharacterWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (nullptr != HPProgressBar)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
