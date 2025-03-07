// Fill out your copyright notice in the Description page of Project Settings.


#include "JHPlayerCharacter.h"
#include "JHAnimInstance.h"
#include "JHPlayerController.h"
#include "InputMappingContext.h"
#include "JHInputData.h"
#include "JHWeapon.h"
#include "EnhancedInputComponent.h"
#include "JHCombatComponent.h"
#include "EnhancedInputSubsystems.h"
#include "JHCharacterStatComponent.h"
#include "Components/WidgetComponent.h"
#include "JHCharacterWidget.h"
#include "Kismet/KismetMathLibrary.h"

AJHPlayerCharacter::AJHPlayerCharacter()
    : CanExecuteNextCombo(false)
    , IsComboInputOn(false)
    , CurrentCombo(0)
    , MaxCombo(3)
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
    Combat = CreateDefaultSubobject<UJHCombatComponent>(TEXT("COMBAT"));
    CharacterStat = CreateDefaultSubobject<UJHCharacterStatComponent>(TEXT("CHARACTERSTAT"));
    HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));

    HPBarWidget->SetupAttachment(GetMesh());

    HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
    HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
    static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));
    if (UI_HUD.Succeeded())
    {
        HPBarWidget->SetWidgetClass(UI_HUD.Class);
        HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
    }

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
    HPBarWidget->SetHiddenInGame(true);
}

void AJHPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    // PlayerController 가져오기
    JHPlayerController = Cast<AJHPlayerController>(GetController());
    JHCHECK(JHPlayerController);
    if (JHPlayerController)
    {
        UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(JHPlayerController->GetLocalPlayer());

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
    JHCHECK(JHAnimInstance)
    JHAnimInstance->OnMontageEnded.AddDynamic(Combat, &UJHCombatComponent::OnAttackMontageEnded);
    JHAnimInstance->OnNextAttackCheck.AddLambda([this, JHAnimInstance]() -> void {
        CanExecuteNextCombo = false;
        if (IsComboInputOn)
        {
            AttackStartComboState();
            JHAnimInstance->JumpToAttackMontageSection(CurrentCombo);
        }
    });

    JHAnimInstance->OnApplyDamage.AddLambda([this]()
        {
            FAttackInfo AttackInfo{}; // TEMP
            AttackInfo.Damage = CharacterStat->GetAttack();
            AttackInfo.Radius = 30.0f;
            AttackInfo.Range = 200.0f;

            Combat->ApplyDamage(AttackInfo);
        });

    Combat->OnAttackEnd.AddUObject(this, &AJHPlayerCharacter::AttackEndComboState);
    CharacterStat->OnHPIsZero.AddUObject(this, &AJHPlayerCharacter::Die);
}

float AJHPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    JHLOG(Warning, TEXT("Actor: %s took Damage: %f"), *GetName(), FinalDamage);

    CharacterStat->SetDamage(FinalDamage);
    return FinalDamage;
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

void AJHPlayerCharacter::SetWeapon(AJHWeapon* NewWeapon)
{
    FName WeaponSocket(TEXT("hand_rSocket"));
    if (nullptr != NewWeapon)
    {
        NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
        NewWeapon->SetOwner(this);
        Combat->SetWeapon(NewWeapon);
    }
}

void AJHPlayerCharacter::Attack()
{
    if (Combat->IsAttacking())
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
        Combat->SetAttacking(true);
    }
}

void AJHPlayerCharacter::Die()
{
    UJHAnimInstance* JHAnimInstance = Cast<UJHAnimInstance>(GetMesh()->GetAnimInstance());
    JHCHECK(JHAnimInstance);
    if (JHAnimInstance)
    {
        JHAnimInstance->SetDeadAnim();
        SetActorEnableCollision(false);
    }
}

bool AJHPlayerCharacter::CanSetWeapon()
{
    return Combat->CanSetWeapon();
}
