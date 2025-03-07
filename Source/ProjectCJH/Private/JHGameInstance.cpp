// Fill out your copyright notice in the Description page of Project Settings.


#include "JHGameInstance.h"

UJHGameInstance::UJHGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/GameData/JHCharacterData.JHCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_JHCHARACTER(*CharacterDataPath);
	JHCHECK(DT_JHCHARACTER.Succeeded());
	JHCharacterTable = DT_JHCHARACTER.Object;
	JHCHECK((JHCharacterTable->GetRowMap().Num() > 0));
}

void UJHGameInstance::Init()
{
	Super::Init();

}

FJHCharacterData* UJHGameInstance::GetJHCharacterData(int32 Level)
{
	return JHCharacterTable->FindRow<FJHCharacterData>(*FString::FromInt(Level), TEXT(""));
}
