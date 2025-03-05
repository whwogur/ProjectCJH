// Fill out your copyright notice in the Description page of Project Settings.


#include "JHSection.h"
#include "Engine/OverlapResult.h"
#include "JHCharacter.h"
#include "LootBox.h"
// Sets default values
AJHSection::AJHSection()
	: CurrentState(ESectionState::READY)
	, bNoBattle(false)
	, EnemySpawnTime(2.0f)
	, LootBoxSpawnTime(5.0f)
	, SpawnNPCTimerHandle{}
	, SpawnLootBoxTimerHandle{}
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	FString AssetPath = TEXT("/Game/Meshes/SM_Map.SM_Map");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_MAP(*AssetPath);
	if (SM_MAP.Succeeded())
	{
		Mesh->SetStaticMesh(SM_MAP.Object);
	}
	else
	{
		JHLOG(Error, TEXT("Failed to load staticmesh asset: %s"), *AssetPath);
	}

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Trigger->SetBoxExtent(FVector(1900.0f, 1900.0f, 300.0f));
	Trigger->SetupAttachment(RootComponent);
	Trigger->SetRelativeLocation(FVector(0.0f, 0.0f, 250.0f));
	Trigger->SetCollisionProfileName(TEXT("JHTrigger"));

	FString PortalAssetPath = TEXT("/Game/SICKA_DYNASTY/StaticMeshes/SM_FloorSet_Pieces11.SM_FloorSet_Pieces11");
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_PORTAL(*PortalAssetPath);
	if (!SM_PORTAL.Succeeded())
	{
		JHLOG(Error, TEXT("Failed to load staticmesh asset: %s"), *PortalAssetPath);
	}

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AJHSection::OnMapTriggerBeginOverlap);

	static FName PortalSockets[] = { { TEXT("+XPortal")}, {TEXT("-XPortal")}, {TEXT("+YPortal")}, {TEXT("-YPortal")} };
	for (FName PortalSocket : PortalSockets)
	{
		JHCHECK(Mesh->DoesSocketExist(PortalSocket));
		UStaticMeshComponent* NewPortal = CreateDefaultSubobject<UStaticMeshComponent>(*PortalSocket.ToString());
		NewPortal->SetStaticMesh(SM_PORTAL.Object);
		NewPortal->SetupAttachment(RootComponent, PortalSocket);
		NewPortal->SetRelativeLocation(FVector(0.0f, 0.0f, 140.0f));
		PortalMeshes.Add(NewPortal);

		UBoxComponent* NewPortalTrigger = CreateDefaultSubobject<UBoxComponent>(*PortalSocket.ToString().Append(TEXT("Trigger")));
		NewPortalTrigger->SetBoxExtent(FVector(150.0f, 150.0f, 300.0f));
		NewPortalTrigger->SetupAttachment(RootComponent, PortalSocket);
		NewPortalTrigger->SetRelativeLocation(FVector(0.0f, 0.0f, 200.0f));
		NewPortalTrigger->SetCollisionProfileName(TEXT("JHTrigger"));
		PortalTriggers.Add(NewPortalTrigger);

		NewPortalTrigger->OnComponentBeginOverlap.AddDynamic(this, &AJHSection::OnPortalTriggerBeginOverlap);
		NewPortalTrigger->ComponentTags.Add(PortalSocket);
	}
}

void AJHSection::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

// Called when the game starts or when spawned
void AJHSection::BeginPlay()
{
	Super::BeginPlay();
	
	SetState(bNoBattle ? ESectionState::COMPLETE : ESectionState::READY);
}

void AJHSection::SetState(ESectionState NewState)
{
	switch (NewState)
	{
	case ESectionState::READY:
	{
		Trigger->SetCollisionProfileName(TEXT("JHTrigger"));
		for (UBoxComponent* PortalTrigger : PortalTriggers)
		{
			PortalTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}

		OperatePortal(true);
		break;
	}
	case ESectionState::BATTLE:
	{
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* PortalTrigger : PortalTriggers)
		{
			PortalTrigger->SetCollisionProfileName(TEXT("NoCollision"));
		}

		OperatePortal(false);

		GetWorld()->GetTimerManager().SetTimer(SpawnNPCTimerHandle, FTimerDelegate::CreateUObject(this, &AJHSection::OnNPCSpawn), EnemySpawnTime, false);

		GetWorld()->GetTimerManager().SetTimer(SpawnLootBoxTimerHandle, FTimerDelegate::CreateLambda([this]() -> void {
			FVector2D RandXY = FMath::RandPointInCircle(1600.f);
			GetWorld()->SpawnActor<ALootBox>(GetActorLocation() + FVector(RandXY, 30.0f), FRotator::ZeroRotator);
		}), LootBoxSpawnTime, false);

		break;
	}
	case ESectionState::COMPLETE:
	{
		Trigger->SetCollisionProfileName(TEXT("NoCollision"));
		for (UBoxComponent* PortalTrigger : PortalTriggers)
		{
			PortalTrigger->SetCollisionProfileName(TEXT("JHTrigger"));
		}

		OperatePortal(true);
		break;
	}
	}

	CurrentState = NewState;
}

void AJHSection::OperatePortal(bool bOpen)
{
	for (UStaticMeshComponent* Portal : PortalMeshes)
	{
		Portal->SetRelativeRotation(bOpen ? FRotator(0.0f, -90.0f, 0.0f) : FRotator::ZeroRotator);
	}
}

void AJHSection::OnNPCSpawn()
{
	GetWorld()->SpawnActor<AJHCharacter>(GetActorLocation() + FVector::UpVector * 90.0f, FRotator::ZeroRotator);
}

void AJHSection::OnMapTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//JHLOG(Warning, TEXT("%d"), CurrentState);
	if (ESectionState::READY == CurrentState)
	{
		SetState(ESectionState::BATTLE);
	}
	//JHLOG(Warning, TEXT("%d"), CurrentState);
}

void AJHSection::OnPortalTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	JHCHECK(OverlappedComponent->ComponentTags.Num() == 1);
	FName ComponentTag = OverlappedComponent->ComponentTags[0];
	FName SocketName = FName(*ComponentTag.ToString().Left(2));
	//JHLOG(Warning, TEXT("ComponentTag: %s, SocketName: %s, OtherActor: %s"), *ComponentTag.ToString(), *SocketName.ToString(), *OtherActor->GetName());
	if (!Mesh->DoesSocketExist(SocketName))
		return;

	FVector NewLocation = Mesh->GetSocketLocation(SocketName);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, this);
	FCollisionObjectQueryParams ObjectQueryParam(FCollisionObjectQueryParams::InitType::AllObjects);
	bool bResult = GetWorld()->OverlapMultiByObjectType(
		OverlapResults,
		NewLocation,
		FQuat::Identity,
		ObjectQueryParam,
		FCollisionShape::MakeSphere(1800.f),
		CollisionQueryParam
	);

	if (!bResult)
	{
		AJHSection* NewSection = GetWorld()->SpawnActor<AJHSection>(NewLocation, FRotator::ZeroRotator);
		JHLOG(Warning, TEXT("New Section"));
	}
	else
	{
		JHLOG(Warning, TEXT("New Section area is not Empty"));
	}
}

