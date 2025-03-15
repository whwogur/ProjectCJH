// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "GameFramework/Actor.h"
#include "JHPatrolRoute.generated.h"

UCLASS()
class PROJECTCJH_API AJHPatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:
    AJHPatrolRoute();

    // 순찰 경로 인덱스를 증가시키고 방향을 조정하는 함수
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    void IncrementPatrolRoute();

    // 현재 순찰 지점의 월드 위치를 반환하는 함수
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    FVector GetSplinePointAsWorldPosition() const;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
    class USplineComponent* PatrolRoute;

    // 현재 순찰 지점의 인덱스
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
    int32 PatrolIndex;

    // 이동 방향 (1: 정방향, -1: 역방향)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
    int32 Direction;

};
