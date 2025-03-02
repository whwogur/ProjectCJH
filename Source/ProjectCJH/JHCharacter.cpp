// Fill out your copyright notice in the Description page of Project Settings.


#include "JHCharacter.h"
#include "JHInputData.h"
#include "JHAnimInstance.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AJHCharacter::AJHCharacter()
    : IsAttacking(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

    SpringArm->SetupAttachment(GetCapsuleComponent());
    Camera->SetupAttachment(SpringArm);

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.f, 0.0f));
    GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);

    SpringArm->TargetArmLength = 400.0f;
    SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
    SpringArm->bDoCollisionTest = true;
    SpringArm->TargetArmLength = 450.0f;
    SpringArm->bUsePawnControlRotation = true;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 800.0f;

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNY(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
    if (SK_MANNY.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(SK_MANNY.Object);
    }
}

// Called when the game starts or when spawned
void AJHCharacter::BeginPlay()
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

// Called every frame
void AJHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AJHCharacter::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    JHAnim = Cast<UJHAnimInstance>(GetMesh()->GetAnimInstance());
    JHCHECK(JHAnim);

    JHAnim->OnMontageEnded.AddDynamic(this, &AJHCharacter::OnAttackMontageEnded);
}

// Called to bind functionality to input
void AJHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    JHLOG(Warning, TEXT("Setting up Player Input Component"));

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        JHLOG(Warning, TEXT("Binding Input Actions"));
        EnhancedInputComponent->BindAction(InputActions->JumpAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnJumpAction);
        EnhancedInputComponent->BindAction(InputActions->LookAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnLookAction);
        EnhancedInputComponent->BindAction(InputActions->MoveAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnMoveAction);
        EnhancedInputComponent->BindAction(InputActions->AttackAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnAttackAction);
    }
    else
    {
        JHLOG(Error, TEXT("Failed to cast UEnhancedInputComponent!"));
    }

}

void AJHCharacter::OnJumpAction(const FInputActionValue& Value)
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

void AJHCharacter::OnLookAction(const FInputActionValue& Value)
{
    FVector2D LookAxisVector = Value.Get<FVector2D>();
    AddControllerYawInput(LookAxisVector.X);   // 좌우 회전
    AddControllerPitchInput(LookAxisVector.Y); // 상하 회전
}

void AJHCharacter::OnMoveAction(const FInputActionValue& Value)
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

void AJHCharacter::OnAttackAction(const FInputActionValue& Value)
{
    if (IsAttacking)
        return;

    if (Value.Get<bool>())
    {
        IsAttacking = true;
        JHAnim->PlayAttackMontage();
    }
}

void AJHCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    JHCHECK(IsAttacking);
    JHLOG(Warning, TEXT("Attack Ended"));
    IsAttacking = false;
}

void AJHCharacter::Jump()
{
    Super::Jump();
    JHLOG_S(Warning);
}

void AJHCharacter::StopJumping()
{
    Super::StopJumping();
}

void AJHCharacter::AddControllerYawInput(float Val)
{
    Super::AddControllerYawInput(Val);
}

void AJHCharacter::AddControllerPitchInput(float Val)
{
    Super::AddControllerPitchInput(Val);
}

void AJHCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
    Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}