// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/Character.h"
#include "JHCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

class AJHWeapon;

UCLASS()
class PROJECTCJH_API AJHCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJHCharacter();

protected:
	virtual void BeginPlay() override;
public:
	void SetCharacterState(ECharacterState NewState);
	ECharacterState GetCharacterState() const { return CurrentState; }

	virtual void OnAssetLoadCompleted();

	FOnAttackEndDelegate OnAttackEnd;

	UFUNCTION(BlueprintCallable)
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
public:
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "State", Meta = (AllowPrivateAccess = true))
	ECharacterState CurrentState;

protected:
	FSoftObjectPath CharacterAssetToLoad;
	TSharedPtr<struct FStreamableHandle> AssetStreamingHandle;
	uint8 AssetIndex;

	bool IsAttacking;
};
