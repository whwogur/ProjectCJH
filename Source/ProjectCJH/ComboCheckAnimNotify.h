// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ProjectCJH.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "ComboCheckAnimNotify.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNotifiedSignature)

UCLASS()
class PROJECTCJH_API UComboCheckAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FOnNotifiedSignature OnNotified;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
