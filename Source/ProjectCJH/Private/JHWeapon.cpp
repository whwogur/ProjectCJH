// Fill out your copyright notice in the Description page of Project Settings.


#include "JHWeapon.h"

// Sets default values
AJHWeapon::AJHWeapon()
	: AttackRange(200.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SMWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	RootComponent = SMWeapon;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WEAPON(TEXT("/Game/DynamicKatanaAnims/Weapon/Katana/SM_Katana_DK.SM_Katana_DK"));
	if (SM_WEAPON.Succeeded())
	{
		SMWeapon->SetStaticMesh(SM_WEAPON.Object);
	}
	SMWeapon->SetCollisionProfileName(TEXT("NoCollision"));
}

void AJHWeapon::SetMeshVisible(bool Visibility, bool PropagateToChildren)
{
	SMWeapon->SetVisibility(Visibility, PropagateToChildren);
}

// Called when the game starts or when spawned
void AJHWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

