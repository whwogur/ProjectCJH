// Fill out your copyright notice in the Description page of Project Settings.


#include "JHPatrolRoute.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"
// Sets default values
AJHPatrolRoute::AJHPatrolRoute()
{
    // 기본 설정
    PrimaryActorTick.bCanEverTick = false;

    // 스플라인 컴포넌트 생성
    PatrolRoute = CreateDefaultSubobject<USplineComponent>(TEXT("PatrolRoute"));
    RootComponent = PatrolRoute;

    // 초기 값 설정
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

    // 현재 인덱스에 방향 값을 더함
    PatrolIndex = PatrolIndex + Direction;

    // 스플라인 포인트 수를 가져옴
    int32 NumberOfPoints = PatrolRoute->GetNumberOfSplinePoints();
    int32 MaxIndex = NumberOfPoints - 1;

    // 조건 체크 및 방향 반전
    if (PatrolIndex == MaxIndex)
    {
        Direction = -1; // 끝에 도달하면 역방향으로
    }
    else if (PatrolIndex == 0)
    {
        Direction = 1;  // 시작점에 도달하면 정방향으로
    }
}

FVector AJHPatrolRoute::GetSplinePointAsWorldPosition() const
{
    if (!PatrolRoute)
    {
        JHLOG_S(Error);
        return FVector::ZeroVector;
    }

    // 현재 PatrolIndex에서 스플라인의 월드 위치 반환
    return PatrolRoute->GetLocationAtSplinePoint(PatrolIndex, ESplineCoordinateSpace::World);
}