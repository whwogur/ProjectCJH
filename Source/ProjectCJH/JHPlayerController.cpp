// Fill out your copyright notice in the Description page of Project Settings.


#include "JHPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

void AJHPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	JHLOG_S(Warning);
}

void AJHPlayerController::BeginPlay()
{
	Super::BeginPlay();
    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
	SetupInputBindings();
}

void AJHPlayerController::SetupInputBindings()
{
    // 로컬 플레이어 서브시스템 가져오기
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        // Input Mapping Context 추가
        if (InputMappingContext)
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }

        // PlayerController의 InputComponent를 사용해 바인딩 (Pawn 없이 직접 처리)
        if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
        {
            if (MoveAction)
            {
                EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJHPlayerController::OnMove);
            }
        }
    }
}

void AJHPlayerController::OnMove(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    JHLOG(Warning, TEXT("Move Input: %s"), *MovementVector.ToString());
}