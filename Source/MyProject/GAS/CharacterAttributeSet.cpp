// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include <Net/UnrealNetwork.h>
 
UCharacterAttributeSet::UCharacterAttributeSet()
{
}


void UCharacterAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	//Boundaries
	if (Attribute == GetHelthAttribute()) {
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHelth());
	}
}

void UCharacterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetDamageAttribute()) {
		const float newDamage = GetDamage();
		SetDamage(0.0f);
		if (newDamage > 0.0f) {
			const float newHelth = GetHelth() - newDamage;
			SetHelth(FMath::Clamp(newHelth, 0.0f, GetMaxHelth()));
		}
	}
}

void UCharacterAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Helth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxHelth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, Stamina, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UCharacterAttributeSet, MaxStamina, COND_None, REPNOTIFY_Always);
}


void UCharacterAttributeSet::OnRep_Helth(const FGameplayAttributeData& OldHelth)
{
	//Attribute being Modified
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Helth, OldHelth);
}

void UCharacterAttributeSet::OnRep_HelthMax(const FGameplayAttributeData& OldHelthMax)
{
	//Attribute being Modified
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxHelth, OldHelthMax);
}

void UCharacterAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	//Attribute being Modified
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, Stamina, OldStamina);
}

void UCharacterAttributeSet::OnRep_StaminaMax(const FGameplayAttributeData& OldStaminaMax)
{
	//Attribute being Modified
	GAMEPLAYATTRIBUTE_REPNOTIFY(UCharacterAttributeSet, MaxStamina, OldStaminaMax);
}
