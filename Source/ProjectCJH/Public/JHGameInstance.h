// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Engine/StreamableManager.h"
#include "JHGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FJHCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FJHCharacterData()
		: Level(1)
		, MaxHP(100.0f)
		, Attack(10.0f)
		, DropExp(10)
		, NextExp(30)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};

UCLASS()
class PROJECTCJH_API UJHGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UJHGameInstance();

	virtual void Init() override;

public:
	FJHCharacterData* GetJHCharacterData(int32 Level);

	FStreamableManager StreamableManager;
private:
	UPROPERTY()
	class UDataTable* JHCharacterTable;
};
