// Fill out your copyright notice in the Description page of Project Settings.


#include "JHCharacter.h"
#include "JHAnimInstance.h"

#include "JHCombatComponent.h"

#include "JHAIController.h"
//#include "JHGameInstance.h"

// Sets default values
AJHCharacter::AJHCharacter()
    : CharacterAssetToLoad(FSoftObjectPath(nullptr))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 400.0f;

    GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.f, 0.0f));
    GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
    GetCapsuleComponent()->SetCollisionProfileName(TEXT("JHCharacter"));

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

    //SetActorHiddenInGame(true);
    //SetCanBeDamaged(false);
}