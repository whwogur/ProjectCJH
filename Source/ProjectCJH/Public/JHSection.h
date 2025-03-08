// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/Actor.h"
#include "JHSection.generated.h"

UCLASS()
class PROJECTCJH_API AJHSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJHSection();
	virtual void OnConstruction(const FTransform& Transform) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
private:
	enum class ESectionState : uint8
	{
		READY = 0,
		BATTLE,
		COMPLETE
	};

	void SetState(ESectionState NewState);
	ESectionState CurrentState;
	
	void OperatePortal(bool bOpen = true);

	void OnNPCSpawn();

	UFUNCTION()
	void OnMapTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPortalTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnKeyNPCDestroyed(AActor* DestroyedActor);

private:
	UPROPERTY(VisibleAnywhere, Category = "Mesh", Meta=(AllowPrivateAccess = true))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, Category = "Mesh", Meta = (AllowPrivateAccess = true))
	TArray<UStaticMeshComponent*> PortalMeshes;

	UPROPERTY(VisibleAnywhere, Category = "Trigger", Meta = (AllowPrivateAccess = true))
	TArray<UBoxComponent*> PortalTriggers;

	UPROPERTY(VisibleAnywhere, Category = "Trigger", Meta = (AllowPrivateAccess = true))
	UBoxComponent* Trigger;

	UPROPERTY(EditAnywhere, Category = "State", Meta = (AllowPrivateAccess = true))
	bool bNoBattle;

	UPROPERTY(EditAnywhere, Category = "Spawn", Meta = (AllowPrivateAccess = true))
	float EnemySpawnTime;

	UPROPERTY(EditAnywhere, Category = "Spawn", Meta = (AllowPrivateAccess = true))
	float LootBoxSpawnTime;

	FTimerHandle SpawnNPCTimerHandle;
	FTimerHandle SpawnLootBoxTimerHandle;
};
