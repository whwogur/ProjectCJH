// Fill out your copyright notice in the Description page of Project Settings.


#include "JHCharacter.h"
#include "JHInputData.h"
#include "JHAnimInstance.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"
#include "JHWeapon.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AJHCharacter::AJHCharacter()
    : IsAttacking(false)
    , CurrentCombo(0)
    , MaxCombo(3)
    , AttackRange(200.0f)
    , AttackRadius(50.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

    SpringArm->SetupAttachment(GetCapsuleComponent());
    Camera->SetupAttachment(SpringArm);

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.f, 0.0f));
    GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("JHCharacter"));

    SpringArm->TargetArmLength = 400.0f;
    SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
    SpringArm->bDoCollisionTest = true;
    SpringArm->TargetArmLength = 450.0f;
    SpringArm->bUsePawnControlRotation = true;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 400.0f;

    // 디폴트 스켈메쉬
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNY(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
    if (SK_MANNY.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(SK_MANNY.Object);
    }

    AttackEndComboState();
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

    // 무기 장착
    FName WeaponSocket(TEXT("hand_rSocket"));
    AJHWeapon* CurWeapon = GetWorld()->SpawnActor<AJHWeapon>(FVector::ZeroVector, FRotator::ZeroRotator);
    if (nullptr != CurWeapon)
    {
        CurWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
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

    JHLOG(Warning, TEXT("Binding OnMontageEnded"));
    JHAnim->OnMontageEnded.AddDynamic(this, &AJHCharacter::OnAttackMontageEnded);
    JHAnim->OnNextAttackCheck.AddLambda([this]() -> void {
        JHLOG(Warning, TEXT("OnNextAttackCheck"));
        CanExecuteNextCombo = false;
        if (IsComboInputOn)
        {
            AttackStartComboState();
            JHAnim->JumpToAttackMontageSection(CurrentCombo);
        }
    });
    JHAnim->OnApplyDamage.AddUObject(this, &AJHCharacter::ApplyDamage);
}

// Called to bind functionality to input
void AJHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    JHLOG(Warning, TEXT("Setting up Player Input Component"));

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(InputActions->JumpAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnJumpAction);
        EnhancedInputComponent->BindAction(InputActions->LookAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnLookAction);
        EnhancedInputComponent->BindAction(InputActions->MoveAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnMoveAction);
        EnhancedInputComponent->BindAction(InputActions->AttackAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnAttackAction);
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
    if (Value.Get<bool>())
    {
        JHLOG(Error, TEXT("Combo: %d"), CurrentCombo);
        if (IsAttacking)
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
            JHAnim->PlayAttackMontage();
            JHAnim->JumpToAttackMontageSection(CurrentCombo);
            IsAttacking = true;
        }
    }
}

void AJHCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    JHCHECK(IsAttacking);
    JHCHECK((CurrentCombo > 0));
    JHLOG(Warning, TEXT("Attack Ended"));
    IsAttacking = false;
    AttackEndComboState();
}

void AJHCharacter::AttackStartComboState()
{
    CanExecuteNextCombo = true;
    IsComboInputOn = false;
    JHCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
    CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
}

void AJHCharacter::AttackEndComboState()
{
    IsComboInputOn = false;
    CanExecuteNextCombo = false;
    CurrentCombo = 0;
}

void AJHCharacter::ApplyDamage()
{
    FHitResult HitResult;
    FCollisionQueryParams Params(NAME_None, false, this);
    bool bResult = GetWorld()->SweepSingleByChannel(
        HitResult,
        GetActorLocation(),
        GetActorLocation() + GetActorForwardVector() * 200.0f,
        FQuat::Identity,
        ECollisionChannel::ECC_GameTraceChannel2,
        FCollisionShape::MakeSphere(50.0f),
        Params
    );

#if ENABLE_DRAW_DEBUG
    FVector TraceVec = GetActorForwardVector() * AttackRange;
    FVector Center = GetActorLocation() + TraceVec * 0.5f;
    float HalfHeight = AttackRange * 0.5f + AttackRadius;
    FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
    FColor DrawColor = bResult ? FColor::Red : FColor::Green;
    float DebugLifeTime = 2.0f;

    DrawDebugCapsule(GetWorld(),
        Center,
        HalfHeight,
        AttackRadius,
        CapsuleRot,
        DrawColor,
        false,
        DebugLifeTime);
#endif

    if (bResult)
    {
        if (::IsValid(HitResult.GetActor()))
        {
            JHLOG(Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
        }

        FDamageEvent DamageEvent;
        HitResult.GetActor()->TakeDamage(50.0f, DamageEvent, GetController(), this);
    }
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

float AJHCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    JHLOG(Warning, TEXT("Actor: %s took Damage: %f"), *GetName(), FinalDamage);

    if (FinalDamage > 0.0f)
    {
        JHAnim->SetDeadAnim();
        SetActorEnableCollision(false);
    }
    return FinalDamage;
}
