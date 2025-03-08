// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/Actor.h"
#include "JHWeapon.generated.h"

UCLASS()
class PROJECTCJH_API AJHWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJHWeapon();

public:
	float GetAttackRange() const { return AttackRange; }
public:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UStaticMeshComponent* Weapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackRange;
};
