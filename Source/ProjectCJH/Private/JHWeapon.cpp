// Fill out your copyright notice in the Description page of Project Settings.


#include "JHWeapon.h"

// Sets default values
AJHWeapon::AJHWeapon()
	: AttackRange(200.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	RootComponent = Weapon;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_WEAPON(TEXT("/Game/DynamicKatanaAnims/Weapon/Katana/SM_Katana_DK.SM_Katana_DK"));
	if (SM_WEAPON.Succeeded())
	{
		Weapon->SetStaticMesh(SM_WEAPON.Object);
	}
	Weapon->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AJHWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

