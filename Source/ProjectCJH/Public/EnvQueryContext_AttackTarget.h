// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_AttackTarget.generated.h"

UCLASS()
class PROJECTCJH_API UEnvQueryContext_AttackTarget : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:
	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;

private:
	static const FName AttackTargetKey;
};
