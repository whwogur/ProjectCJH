// Fill out your copyright notice in the Description page of Project Settings.


#include "JHCharacter.h"
#include "JHInputData.h"
#include "JHAnimInstance.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/DamageEvents.h"
#include "JHWeapon.h"
#include "JHCharacterStatComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/WidgetComponent.h"
#include "JHCharacterWidget.h"
#include "JHAIController.h"
//#include "JHGameInstance.h"

// Sets default values
AJHCharacter::AJHCharacter()
    : IsAttacking(false)
    , CurrentCombo(0)
    , MaxCombo(3)
    , AttackRange(200.0f)
    , AttackRadius(50.0f)
    , CharacterAssetToLoad(FSoftObjectPath(nullptr))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
    CharacterStat = CreateDefaultSubobject<UJHCharacterStatComponent>(TEXT("CHARACTERSTAT"));
    HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

    SpringArm->SetupAttachment(GetCapsuleComponent());
    Camera->SetupAttachment(SpringArm);
    HPBarWidget->SetupAttachment(GetMesh());

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.f, 0.0f));
    GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("JHCharacter"));

    SpringArm->TargetArmLength = 400.0f;
    SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
    SpringArm->bDoCollisionTest = true;
    SpringArm->TargetArmLength = 450.0f;
    SpringArm->bUsePawnControlRotation = true;

    HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
    HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
    static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));
    if (UI_HUD.Succeeded())
    {
        HPBarWidget->SetWidgetClass(UI_HUD.Class);
        HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
    }

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 400.0f;

    // 디폴트 스켈메쉬
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MANNY(TEXT("/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny"));
    if (SK_MANNY.Succeeded())
    {
        GetMesh()->SetSkeletalMesh(SK_MANNY.Object);
    }

    AIControllerClass = AJHAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

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

    UJHCharacterWidget* CharacterWidget = Cast<UJHCharacterWidget>(HPBarWidget->GetUserWidgetObject());
    if (nullptr != CharacterWidget)
    {
        CharacterWidget->BindCharacterStat(CharacterStat);
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
    JHAnim->OnNextAttackCheck.AddLambda([this]() -> void {
        CanExecuteNextCombo = false;
        if (IsComboInputOn)
        {
            AttackStartComboState();
            JHAnim->JumpToAttackMontageSection(CurrentCombo);
        }
    });
    JHAnim->OnApplyDamage.AddUObject(this, &AJHCharacter::ApplyDamage);

    CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
        JHAnim->SetDeadAnim();
        SetActorEnableCollision(false);
    });
}

// Called to bind functionality to input
void AJHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(InputActions->JumpAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnJumpAction);
        EnhancedInputComponent->BindAction(InputActions->LookAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnLookAction);
        EnhancedInputComponent->BindAction(InputActions->MoveAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnMoveAction);
        EnhancedInputComponent->BindAction(InputActions->AttackAction, ETriggerEvent::Triggered, this, &AJHCharacter::OnAttackAction);
    }
}

bool AJHCharacter::CanSetWeapon()
{
    return (nullptr == CurrentWeapon);
}

void AJHCharacter::SetWeapon(AJHWeapon* NewWeapon)
{
    JHCHECK((nullptr != NewWeapon && nullptr == CurrentWeapon));

    FName WeaponSocket(TEXT("hand_rSocket"));
    if (nullptr != NewWeapon)
    {
        NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
        NewWeapon->SetOwner(this);
        CurrentWeapon = NewWeapon;
    }
}

void AJHCharacter::Attack()
{
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
        Attack();
    }
}

void AJHCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    JHCHECK(IsAttacking);
    JHCHECK((CurrentCombo > 0));
    IsAttacking = false;
    AttackEndComboState();
    OnAttackEnd.Broadcast();
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
        HitResult.GetActor()->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
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

    CharacterStat->SetDamage(FinalDamage);
    return FinalDamage;
}