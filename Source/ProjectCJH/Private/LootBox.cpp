// Fill out your copyright notice in the Description page of Project Settings.


#include "LootBox.h"
#include "JHWeapon.h"
#include "JHCharacter.h"

// Sets default values
ALootBox::ALootBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 24.0f, 30.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/Fab/Chest/chest.chest"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}
	Box->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	Trigger->SetCollisionProfileName(TEXT("Loot"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponItemClass = AJHWeapon::StaticClass();
}

// Called when the game starts or when spawned
void ALootBox::BeginPlay()
{
	Super::BeginPlay();	
}

void ALootBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ALootBox::OnCharacterOverlap);
}

void ALootBox::OnCharacterOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweeoResult)
{
	JHLOG_S(Warning);

	AJHCharacter* Character = Cast<AJHCharacter>(OtherActor);
	JHCHECK(Character);

	if (nullptr != Character && nullptr != WeaponItemClass)
	{
		if (Character->CanSetWeapon())
		{
			AJHWeapon* NewWeapon = GetWorld()->SpawnActor<AJHWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator);
			Character->SetWeapon(NewWeapon);
		}
		else
		{
			JHLOG(Warning, TEXT("%s Can't Equip Weapon"), *Character->GetName());
		}
	}
}

