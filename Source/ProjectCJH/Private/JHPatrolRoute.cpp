// Fill out your copyright notice in the Description page of Project Settings.


#include "JHPatrolRoute.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"
// Sets default values
AJHPatrolRoute::AJHPatrolRoute()
{
    // �⺻ ����
    PrimaryActorTick.bCanEverTick = false;

    // ���ö��� ������Ʈ ����
    PatrolRoute = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolRoute"));
    RootComponent = PatrolRoute;

    // �ʱ� �� ����
    PatrolIndex = 0;
    Direction = 1;
}

void AJHPatrolRoute::IncrementPatrolRoute()
{
    if (!PatrolRoute)
    {
        JHLOG_S(Error);
        return;
    }

    // ���� �ε����� ���� ���� ����
    PatrolIndex = PatrolIndex + Direction;

    // ���ö��� ����Ʈ ���� ������
    int32 NumberOfPoints = PatrolRoute->GetNumberOfSplinePoints();
    int32 MaxIndex = NumberOfPoints - 1;

    // ���� üũ �� ���� ����
    if (PatrolIndex == MaxIndex)
    {
        Direction = -1; // ���� �����ϸ� ����������
    }
    else if (PatrolIndex == 0)
    {
        Direction = 1;  // �������� �����ϸ� ����������
    }
}

FVector AJHPatrolRoute::GetSplinePointAsWorldPosition() const
{
    if (!PatrolRoute)
    {
        JHLOG_S(Error);
        return FVector::ZeroVector;
    }

    // ���� PatrolIndex���� ���ö����� ���� ��ġ ��ȯ
    return PatrolRoute->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}