// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Components/ActorComponent.h"
#include "JHCombatComponent.generated.h"


USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_BODY()

public:
	FAttackInfo()
		: Damage(0)
		, Radius(0)
		, Range(0)
	{}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Range;

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCJH_API UJHCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJHCombatComponent();

public:
	void ApplyDamage(const FAttackInfo& AttackInfo);
};
