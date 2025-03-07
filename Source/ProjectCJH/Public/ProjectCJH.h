// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "EngineMinimal.h"

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	PREINIT,
	LOADING,
	READY,
	DEAD
};

DECLARE_LOG_CATEGORY_EXTERN(ProjectCJH, Log, All);
#define JHLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define JHLOG_S(Verbosity) UE_LOG(ProjectCJH, Verbosity, TEXT("%s"), *JHLOG_CALLINFO)
#define JHLOG(Verbosity, Format, ...) UE_LOG(ProjectCJH, Verbosity, TEXT("%s%s"), *JHLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

#define JHCHECK(Expr, ...)\
{\
	if(!Expr)\
	{\
		JHLOG(Error, TEXT("ASSERTION: %s"), TEXT(","#Expr","));\
		return __VA_ARGS__;\
	}\
}