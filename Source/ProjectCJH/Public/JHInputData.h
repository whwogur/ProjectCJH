// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "JHInputData.generated.h"

class UInputAction;

UCLASS()
class PROJECTCJH_API UJHInputData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* AttackAction;
};
