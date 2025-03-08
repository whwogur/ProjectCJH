// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "JHPlayerController.generated.h"

class UJHHUDWidget;

UCLASS()
class PROJECTCJH_API AJHPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AJHPlayerController();
public:
	UJHHUDWidget* GetHUDWidget() const;
	void EnemyKill(class AJHEnemyBase*) const;
	void AddGameScore() const;
protected:
	virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UJHHUDWidget> HUDWidgetClass;

private:
	UPROPERTY()
	UJHHUDWidget* HUDWidget;

	UPROPERTY()
	class AJHPlayerState* JHPlayerState;
};
