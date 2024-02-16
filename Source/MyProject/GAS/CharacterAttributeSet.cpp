// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
 
UCharacterAttributeSet::UCharacterAttributeSet()
{
}

void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHelthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHelth());
	}
	if (Attribute == GetDamageAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHelth());
	}
}

