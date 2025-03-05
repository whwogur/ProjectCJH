// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JHCharacterSetting.generated.h"

UCLASS(config=ProjectCJH)
class PROJECTCJHSETTING_API UJHCharacterSetting : public UObject
{
	GENERATED_BODY()
	
public:
	UJHCharacterSetting();

	UPROPERTY(config)
	TArray<FSoftObjectPath> CharacterAssets;
};
