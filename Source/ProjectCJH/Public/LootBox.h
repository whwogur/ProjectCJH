// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/Actor.h"
#include "LootBox.generated.h"

UCLASS()
class PROJECTCJH_API ALootBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:
	UPROPERTY(VisibleAnywhere, Category = "Box")
	UBoxComponent* Trigger;

	UPROPERTY(VisibleAnywhere, Category = "Box")
	UStaticMeshComponent* Box;

	UPROPERTY(EditInstanceOnly, Category = "Box")
	TSubclassOf<class AJHWeapon> WeaponItemClass;

private:
	UFUNCTION()
	void OnPlayerOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweeoResult);
};
