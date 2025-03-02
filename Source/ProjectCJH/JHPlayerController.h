// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "JHPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCJH_API AJHPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void PostInitializeComponents() override;
    virtual void BeginPlay() override;
};
