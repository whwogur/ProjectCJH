// Fill out your copyright notice in the Description page of Project Settings.


#include "JHEnemyBase.h"
#include "JHAnimInstance.h"

#include "JHAIController.h"

#include "Components/WidgetComponent.h"
#include "JHCharacterWidget.h"
#include "JHCombatComponent.h"
#include "JHCharacterStatComponent.h"

#include "JHWeapon.h"

AJHEnemyBase::AJHEnemyBase()
{
    AssetIndex = 0;


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

    AIControllerClass = AJHAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void AJHEnemyBase::BeginPlay()
{
    Super::BeginPlay();
    // PlayerController 가져오기
    JHAIController = Cast<AJHAIController>(GetController());
    JHCHECK(JHAIController);

    UJHCharacterWidget* CharacterWidget = Cast<UJHCharacterWidget>(HPBarWidget->GetUserWidgetObject());
    if (nullptr != CharacterWidget)
    {
        CharacterWidget->BindCharacterStat(CharacterStat);
    }
}

void AJHEnemyBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();
    

    CharacterStat->OnHPIsZero.AddUObject(this, &AJHEnemyBase::Die);
}

void AJHEnemyBase::SetWeapon(AJHWeapon* NewWeapon)
{
    FName WeaponSocket(TEXT("hand_rSocket"));
    if (nullptr != NewWeapon)
    {
        NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
        NewWeapon->SetOwner(this);
        Combat->SetWeapon(NewWeapon);
    }
}

void AJHEnemyBase::Attack()
{
    UJHAnimInstance* JHAnimInstance = StaticCast<UJHAnimInstance*>(GetMesh()->GetAnimInstance());
    JHAnimInstance->PlayAttackMontage();
    Combat->SetAttacking(true);
}

void AJHEnemyBase::Die()
{
    UJHAnimInstance* JHAnimInstance = Cast<UJHAnimInstance>(GetMesh()->GetAnimInstance());
    JHCHECK(JHAnimInstance);
    if (JHAnimInstance)
    {
        JHAnimInstance->SetDeadAnim();
        SetActorEnableCollision(false);
    }
}

bool AJHEnemyBase::CanSetWeapon()
{
	return Combat->CanSetWeapon();
}

float AJHEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    JHLOG(Warning, TEXT("Actor: %s took Damage: %f"), *GetName(), FinalDamage);

    CharacterStat->SetDamage(FinalDamage);
    return FinalDamage;
}

void AJHEnemyBase::OnAssetLoadCompleted()
{
    Super::OnAssetLoadCompleted();
    UJHAnimInstance* JHAnimInstance = Cast<UJHAnimInstance>(GetMesh()->GetAnimInstance());
    JHCHECK(JHAnimInstance)
        JHAnimInstance->OnMontageEnded.AddDynamic(Combat, &UJHCombatComponent::OnAttackMontageEnded);

    JHAnimInstance->OnApplyDamage.AddLambda([this]()
        {
            FAttackInfo AttackInfo{}; // TEMP
            AttackInfo.Damage = CharacterStat->GetAttack();
            AttackInfo.Radius = 30.0f;
            AttackInfo.Range = 200.0f;

            Combat->ApplyDamage(AttackInfo);
    });
}
