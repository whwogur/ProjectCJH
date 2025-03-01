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
    // ���� �÷��̾� ����ý��� ��������
    if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
    {
        // Input Mapping Context �߰�
        if (InputMappingContext)
        {
            Subsystem->AddMappingContext(InputMappingContext, 0);
        }

        // PlayerController�� InputComponent�� ����� ���ε� (Pawn ���� ���� ó��)
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