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

    // ���� ��� �ε����� ������Ű�� ������ �����ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    void IncrementPatrolRoute();

    // ���� ���� ������ ���� ��ġ�� ��ȯ�ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Patrol")
    FVector GetSplinePointAsWorldPosition() const;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
    class USplineComponent* PatrolRoute;

    // ���� ���� ������ �ε���
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
    int32 PatrolIndex;

    // �̵� ���� (1: ������, -1: ������)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Patrol", meta = (AllowPrivateAccess = "true"))
    int32 Direction;

};
