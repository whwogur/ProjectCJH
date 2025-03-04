// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Blueprint/UserWidget.h"
#include "JHCharacterWidget.generated.h"

UCLASS()
class PROJECTCJH_API UJHCharacterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindCharacterStat(class UJHCharacterStatComponent* NewCharacterStat);

private:
	TWeakObjectPtr<class UJHCharacterStatComponent> CurrentCharacterStat;
};
