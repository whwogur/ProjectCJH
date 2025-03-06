// Fill out your copyright notice in the Description page of Project Settings.


#include "JHPlayerCharacter.h"
#include "JHAnimInstance.h"
#include "InputMappingContext.h"
#include "JHInputData.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

AJHPlayerCharacter::AJHPlayerCharacter()
    : CanExecuteNextCombo(false)
    , IsComboInputOn(false)
    , CurrentCombo(0)
    , MaxCombo(3)
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	SpringArm->bDoCollisionTest = true;
	SpringArm->TargetArmLength = 450.0f;
	SpringArm->bUsePawnControlRotation = true;

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMC_PLAYER(TEXT("/Game/Player/Input/IMC_Player.IMC_Player"));
    JHCHECK(IMC_PLAYER.Succeeded());
    if (IMC_PLAYER.Succeeded())
    {
        InputMapping = IMC_PLAYER.Object;
    }

    static ConstructorHelpers::FObjectFinder<UJHInputData> INPUTDATA(TEXT("/Game/Player/Input/Actions/DA_JHPlayerInputConfig.DA_JHPlayerInputConfig"));
    JHCHECK(INPUTDATA.Succeeded());
    if (INPUTDATA.Succeeded())
    {
        InputActions = INPUTDATA.Object;
    }

	AttackEndComboState();
}

void AJHPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    // PlayerController 가져오기
    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (PlayerController)
    {
        UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

        if (InputSubsystem && InputMapping)
        {
            InputSubsystem->AddMappingContext(InputMapping, 0);
        }
    }
}

// Called to bind functionality to input
void AJHPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(InputActions->JumpAction, ETriggerEvent::Triggered, this, &AJHPlayerCharacter::OnJumpAction);
		EnhancedInputComponent->BindAction(InputActions->LookAction, ETriggerEvent::Triggered, this, &AJHPlayerCharacter::OnLookAction);
		EnhancedInputComponent->BindAction(InputActions->MoveAction, ETriggerEvent::Triggered, this, &AJHPlayerCharacter::OnMoveAction);
		EnhancedInputComponent->BindAction(InputActions->AttackAction, ETriggerEvent::Triggered, this, &AJHPlayerCharacter::OnAttackAction);
	}
}

void AJHPlayerCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    UJHAnimInstance* JHAnimInstance = Cast<UJHAnimInstance>(GetMesh()->GetAnimInstance());
    JHAnimInstance->OnNextAttackCheck.AddLambda([this, JHAnimInstance]() -> void {
        CanExecuteNextCombo = false;
        if (IsComboInputOn)
        {
            AttackStartComboState();
            JHAnimInstance->JumpToAttackMontageSection(CurrentCombo);
        }
    });
}

void AJHPlayerCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    Super::OnAttackMontageEnded(Montage, bInterrupted);
    AttackEndComboState();
}


void AJHPlayerCharacter::OnJumpAction(const FInputActionValue& Value)
{
    bool bJumpPressed = Value.Get<bool>();
    if (bJumpPressed)
    {
        Jump();
    }
    else
    {
        StopJumping();
    }
}

void AJHPlayerCharacter::OnLookAction(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);   // 좌우 회전
    AddControllerPitchInput(LookAxisVector.Y); // 상하 회전
}

void AJHPlayerCharacter::OnMoveAction(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    if (FVector2D::ZeroVector != MovementVector)
    {
        FRotator Rotation = GetActorRotation();
        FVector ForwardDirection = UKismetMathLibrary::GetForwardVector(Rotation);
        FVector RightDirection = UKismetMathLibrary::GetRightVector(Rotation);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void AJHPlayerCharacter::OnAttackAction(const FInputActionValue& Value)
{
    if (Value.Get<bool>())
    {
        Attack();
    }
}

void AJHPlayerCharacter::AttackStartComboState()
{
    CanExecuteNextCombo = true;
    IsComboInputOn = false;
    JHCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
    CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AJHPlayerCharacter::AttackEndComboState()
{
    IsComboInputOn = false;
    CanExecuteNextCombo = false;
    CurrentCombo = 0;
}

void AJHPlayerCharacter::AddControllerYawInput(float Val)
{
    Super::AddControllerYawInput(Val);
}

void AJHPlayerCharacter::AddControllerPitchInput(float Val)
{
    Super::AddControllerPitchInput(Val);
}

void AJHPlayerCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
    Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

void AJHPlayerCharacter::Attack()
{
    if (IsAttacking())
    {
        JHCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
        if (CanExecuteNextCombo)
        {
            IsComboInputOn = true;
        }
    }
    else
    {
        JHCHECK((0 == CurrentCombo));
        AttackStartComboState();
        UJHAnimInstance* JHAnimInstance = StaticCast<UJHAnimInstance*>(GetMesh()->GetAnimInstance());
        JHAnimInstance->PlayAttackMontage();
        JHAnimInstance->JumpToAttackMontageSection(CurrentCombo);
        SetAttacking(true);
    }
}