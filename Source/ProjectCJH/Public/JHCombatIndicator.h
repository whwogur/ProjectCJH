// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Blueprint/UserWidget.h"
#include "JHCombatIndicator.generated.h"

UCLASS()
class PROJECTCJH_API UJHCombatIndicator : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetReceivedDamage(float Damage);
protected:
	virtual void NativeConstruct() override;

private:
	UWidgetAnimation* FindWidgetAnimation(FName AnimationName);
private:
	UPROPERTY()
	class UTextBlock* ReceivedDamage = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly , meta = (AllowPrivateAccess = true))
	UWidgetAnimation* WidgetAnimation = nullptr;
};
