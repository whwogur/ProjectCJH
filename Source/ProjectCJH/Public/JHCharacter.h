// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/Character.h"
#include "JHCharacter.generated.h"

class AJHWeapon;

UCLASS()
class PROJECTCJH_API AJHCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJHCharacter();
	void SetCharacterState(ECharacterState NewState) { CurrentState = NewState; };
	ECharacterState GetCharacterState() const { return CurrentState; }

public:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;

private:
	FSoftObjectPath CharacterAssetToLoad;
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;

};
