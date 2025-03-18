// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemyBase.h"
#include "JHAnimInstance.h"

#include "JHAIController.h"

#include "Components/WidgetComponent.h"
#include "JHCharacterWidget.h"
#include "JHCombatComponent.h"
#include "JHCharacterStatComponent.h"
#include "JHPlayerController.h"
#include "JHWeapon.h"
#include "JHCombatIndicator.h"
#include "JHPatrolRoute.h"

AJHEnemyBase::AJHEnemyBase()
    : DeadTimer(5.0f)
    , DeadTimerHandle{}
    , CombatIndicatorUI(nullptr)
{
    AssetIndex = 0;

    Combat = CreateDefaultSubobject<UJHCombatComponent>(TEXT("COMBAT"));
    CharacterStat = CreateDefaultSubobject<UJHCharacterStatComponent>(TEXT("CHARACTERSTAT"));
    HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
    CombatIndicator = CreateDefaultSubobject<UWidgetComponent>(TEXT("COMBATINDICATOR"));

    HPBarWidget->SetupAttachment(GetMesh());
    CombatIndicator->SetupAttachment(GetMesh());

    HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
    HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);

    CombatIndicator->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
    CombatIndicator->SetWidgetSpace(EWidgetSpace::Screen);

    // µðÆúÆ® Anim
    static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_CHARACTER(TEXT("/Game/Player/ABP_JHCharacter.ABP_JHCharacter_C"));
    if (ABP_CHARACTER.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(ABP_CHARACTER.Class);
    }

    static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/UI/UI_HPBar.UI_HPBar_C"));
    if (UI_HUD.Succeeded())
    {
        HPBarWidget->SetWidgetClass(UI_HUD.Class);
        HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
    }

    static ConstructorHelpers::FClassFinder<UUserWidget> UI_DAMAGE(TEXT("/Game/UI/UI_DamageIndicator.UI_DamageIndicator_C"));
    if (UI_DAMAGE.Succeeded())
    {
        CombatIndicator->SetWidgetClass(UI_DAMAGE.Class);
    }

    AIControllerClass = AJHAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AJHEnemyBase::BeginPlay()
{
    Super::BeginPlay();
    // PlayerController
    JHAIController = Cast<AJHAIController>(GetController());
    JHCHECK(JHAIController);

    UJHCharacterWidget* CharacterWidget = Cast<UJHCharacterWidget>(HPBarWidget->GetUserWidgetObject());
    if (nullptr != CharacterWidget)
    {
        CharacterWidget->BindCharacterStat(CharacterStat);
    }

    UJHCombatIndicator* IndicatorUI = Cast<UJHCombatIndicator>(CombatIndicator->GetUserWidgetObject());
    if (nullptr != IndicatorUI)
    {
        CombatIndicatorUI = IndicatorUI;
    }
    JHCHECK(nullptr != CombatIndicatorUI);
    JHCHECK(PatrolRoute);
}

void AJHEnemyBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    

    CharacterStat->OnHPIsZero.AddUObject(this, &AJHEnemyBase::Die);
}

void AJHEnemyBase::SetWeapon(AJHWeapon* NewWeapon, const FName& SocketName)
{
    if (nullptr != NewWeapon)
    {
        NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
        NewWeapon->SetOwner(this);
        Combat->SetWeapon(NewWeapon);
    }
}



void AJHEnemyBase::Die()
{
    UJHAnimInstance* JHAnimInstance = Cast<UJHAnimInstance>(GetMesh()->GetAnimInstance());
    JHCHECK(JHAnimInstance);
    if (JHAnimInstance)
    {
        JHAnimInstance->SetDeadAnim();
    }

    SetActorEnableCollision(false);
    JHAIController->StopAI();
    SetCharacterState(ECharacterState::DEAD);
    HPBarWidget->SetHiddenInGame(true);
    GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
        [this]() -> void { Destroy(); })
    , DeadTimer
    , false);
}

bool AJHEnemyBase::CanSetWeapon()
{
	return Combat->CanSetWeapon();
}

float AJHEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    JHLOG_SIMPLE(TEXT("%s took Damage: %d"), *GetName(), FMath::RoundToInt(FinalDamage));

    CharacterStat->SetDamageReceived(FinalDamage);
    CombatIndicatorUI->SetReceivedDamage(FinalDamage);

    if (CurrentState == ECharacterState::DEAD)
    {
        AJHPlayerController* playerController = Cast<AJHPlayerController>(EventInstigator);
        JHCHECK(playerController, 0.0f);
        playerController->EnemyKill(this);
    }
    
    return FinalDamage;
}

void AJHEnemyBase::OnAssetLoadCompleted()
{
    Super::OnAssetLoadCompleted();
    UJHAnimInstance* JHAnimInstance = Cast<UJHAnimInstance>(GetMesh()->GetAnimInstance());
    JHCHECK(JHAnimInstance);
    

    JHAnimInstance->OnApplyDamage.AddLambda([this]()
        {
            FAttackInfo AttackInfo{};
            AttackInfo.Damage = CharacterStat->GetAttack();
            AttackInfo.Radius = 30.0f;
            AttackInfo.Range = 200.0f;

            Combat->ApplyDamage(AttackInfo);
    });
}

void AJHEnemyBase::OnWeaponEquipCompleted(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted)
        JHLOG(Warning, TEXT("%s interrupted"), *Montage->GetName());

    OnWeaponEquipped.Broadcast();
}

void AJHEnemyBase::OnWeaponSheatheCompleted(UAnimMontage* Montage, bool bInterrupted)
{
    if (bInterrupted)
        JHLOG(Warning, TEXT("%s interrupted"), *Montage->GetName());
    OnWeaponSheathed.Broadcast();
}

int32 AJHEnemyBase::GetExp() const
{
    return CharacterStat->GetDropExp();
}

AJHPatrolRoute* AJHEnemyBase::GetPatrolRoute()
{
    return PatrolRoute;
}

void AJHEnemyBase::MoveWeapon(const FName& SocketName)
{
    AJHWeapon* curWeapon = Combat->GetCurWeapon();
    JHCHECK(curWeapon);
    if (nullptr != curWeapon)
    {
        curWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
    }
}
