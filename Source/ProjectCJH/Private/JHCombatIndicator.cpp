// Fill out your copyright notice in the Description page of Project Settings.


#include "JHCombatIndicator.h"
#include "Components/TextBlock.h"
#include "JHCharacterStatComponent.h"
#include "Animation/WidgetAnimation.h"

void UJHCombatIndicator::SetReceivedDamage(float Damage)
{
	ReceivedDamage->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(Damage))));
	PlayAnimation(WidgetAnimation);
}

UWidgetAnimation* UJHCombatIndicator::FindWidgetAnimation(FName AnimationName)
{
    UWidgetAnimation* FoundAnimation = nullptr;

    FString AnimationNameWithInst = AnimationName.ToString() + TEXT("_INST");

    for (TFieldIterator<FProperty> PropIt(GetClass()); PropIt; ++PropIt)
    {
        FProperty* Property = *PropIt;
        if (Property->GetClass() == FObjectProperty::StaticClass())
        {
            FObjectProperty* ObjectProp = CastField<FObjectProperty>(Property);
            if (ObjectProp->PropertyClass == UWidgetAnimation::StaticClass())
            {
                UWidgetAnimation* Animation = Cast<UWidgetAnimation>(ObjectProp->GetObjectPropertyValue_InContainer(this));
                if (!Animation) continue;

                FName AnimFName = Animation->GetFName();
                JHLOG_SIMPLE(TEXT("Found Animation: %s"), *AnimFName.ToString());

                if (AnimFName == FName(*AnimationNameWithInst) || AnimFName == AnimationName)
                {
                    FoundAnimation = Animation;
                    break;
                }
            }
        }
    }
    return FoundAnimation;
}

void UJHCombatIndicator::NativeConstruct()
{
	Super::NativeConstruct();

	ReceivedDamage = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtDamage")));
	JHCHECK(ReceivedDamage);

	if (!WidgetAnimation)
	{
		WidgetAnimation = FindWidgetAnimation(TEXT("DamageTextAnim"));
	}
    JHCHECK(WidgetAnimation);

}
