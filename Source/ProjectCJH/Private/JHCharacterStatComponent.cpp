// Fill out your copyright notice in the Description page of Project Settings.


#include "JHCharacterStatComponent.h"
#include "JHGameInstance.h"

// Sets default values
UJHCharacterStatComponent::UJHCharacterStatComponent()
	: CurrentStatData(nullptr)
	, Level(1)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;

}

// Called when the game starts or when spawned
void UJHCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UJHCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetNewLevel(Level);
}

void UJHCharacterStatComponent::SetNewLevel(int32 NewLevel)
{
	UJHGameInstance* GameInstance = Cast<UJHGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	JHCHECK(GameInstance);
	CurrentStatData = GameInstance->GetJHCharacterData(NewLevel);
	if (CurrentStatData)
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
	else
	{
		JHLOG(Error, TEXT("Level (%d) data doesn't exist"), NewLevel);
	}
}

void UJHCharacterStatComponent::SetDamageReceived(float NewDamage)
{
	JHCHECK(CurrentStatData);
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

void UJHCharacterStatComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChanged.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHPIsZero.Broadcast();
	}
}

float UJHCharacterStatComponent::GetAttack()
{
	JHCHECK((nullptr != CurrentStatData), 0.0f);

	return CurrentStatData->Attack;
}

float UJHCharacterStatComponent::GetHPRatio()
{
	JHCHECK((nullptr != CurrentStatData), 0.0f);

	return (CurrentStatData->MaxHP < KINDA_SMALL_NUMBER) ? 0.0f : (CurrentHP / CurrentStatData->MaxHP);
}

int32 UJHCharacterStatComponent::GetDropExp() const
{
	return CurrentStatData->DropExp;
}


