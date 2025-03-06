// Fill out your copyright notice in the Description page of Project Settings.


#include "JHCharacter.h"
#include "JHAnimInstance.h"
#include "JHWeapon.h"
#include "JHCharacterStatComponent.h"
#include "JHCombatComponent.h"
#include "Components/WidgetComponent.h"
#include "JHCharacterWidget.h"
#include "JHAIController.h"
//#include "JHGameInstance.h"

// Sets default values
AJHCharacter::AJHCharacter()
    : CharacterAssetToLoad(FSoftObjectPath(nullptr))
    , CurrentWeapon(nullptr)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CharacterStat = CreateDefaultSubobject<UJHCharacterStatComponent>(TEXT("CHARACTERSTAT"));
    Combat = CreateDefaultSubobject<UJHCombatComponent>(TEXT("COMBAT"));
    HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
    HPBarWidget->SetupAttachment(GetMesh());

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.f, 0.0f));
    GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("JHCharacter"));

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

    // 디폴트 Anim
    static ConstructorHelpers::FClassFinder<UAnimInstance> ABP_CHARACTER(TEXT("/Game/Player/ABP_JHCharacter.ABP_JHCharacter_C"));
    if (ABP_CHARACTER.Succeeded())
    {
        GetMesh()->SetAnimInstanceClass(ABP_CHARACTER.Class);
    }
}

// Called when the game starts or when spawned
void AJHCharacter::BeginPlay()
{
	Super::BeginPlay();

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
    JHAnim->OnApplyDamage.AddLambda([this]()
        {
            FAttackInfo AttackInfo{};
            AttackInfo.Damage = CharacterStat->GetAttack();
            AttackInfo.Radius = AttackRadius;
            AttackInfo.Range = AttackRange;

            Combat->ApplyDamage(AttackInfo);
        });

    CharacterStat->OnHPIsZero.AddLambda([this]() -> void {
        JHAnim->SetDeadAnim();
        SetActorEnableCollision(false);
    });
}

void AJHCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    JHCHECK(bAttacking);
    bAttacking = false;
    OnAttackEnd.Broadcast();
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
    bAttacking = true;
}

void AJHCharacter::Jump()
{
    Super::Jump();
}

void AJHCharacter::StopJumping()
{
    Super::StopJumping();
}

float AJHCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    JHLOG(Warning, TEXT("Actor: %s took Damage: %f"), *GetName(), FinalDamage);

    CharacterStat->SetDamage(FinalDamage);
    return FinalDamage;
}