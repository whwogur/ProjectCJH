// Fill out your copyright notice in the Description page of Project Settings.


#include "JHCharacter.h"

#include "JHCharacterSetting.h"
#include "JHGameInstance.h"
// Sets default values
AJHCharacter::AJHCharacter()
    : CharacterAssetToLoad(FSoftObjectPath(nullptr))
    , AssetIndex(0)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->MaxWalkSpeed = 400.0f;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 400.0f;

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.f, 0.0f));
    GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("JHCharacter"));

    SetCharacterState(ECharacterState::LOADING);
}

void AJHCharacter::BeginPlay()
{
    Super::BeginPlay();
    auto DefaultSetting = GetDefault<UJHCharacterSetting>();

    CharacterAssetToLoad = DefaultSetting->CharacterAssets[AssetIndex];
    UJHGameInstance* JHGameInstace = Cast<UJHGameInstance>(GetGameInstance());
    JHCHECK(JHGameInstace);
    AssetStreamingHandle = JHGameInstace->StreamableManager.RequestAsyncLoad(CharacterAssetToLoad, FStreamableDelegate::CreateUObject(this, &AJHCharacter::OnAssetLoadCompleted));
}

void AJHCharacter::SetCharacterState(ECharacterState NewState)
{
    JHCHECK((CurrentState != NewState));
    CurrentState = NewState;

    switch (CurrentState)
    {
    case ECharacterState::LOADING:
    {
        SetActorHiddenInGame(true);
        SetCanBeDamaged(false);
        break;
    }
    case ECharacterState::READY:
    {
        SetActorHiddenInGame(false);
        SetCanBeDamaged(true);
        break;
    }
    case ECharacterState::DEAD:
    {
        SetActorEnableCollision(false);
        SetCanBeDamaged(false);
        break;
    }
    }
}

void AJHCharacter::OnAssetLoadCompleted()
{
    AssetStreamingHandle->ReleaseHandle();
    TSoftObjectPtr<USkeletalMesh> LoadedAssetPath(CharacterAssetToLoad);
    JHCHECK(LoadedAssetPath.IsValid());

    GetMesh()->SetSkeletalMesh(LoadedAssetPath.Get());
    SetCharacterState(ECharacterState::READY);
}

void AJHCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    OnAttackEnd.Broadcast();
}
