// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterStatsAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName)\
        GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
        GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
        GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
        GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class MYPROJECT_API UCharacterStatsAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData Helth;
    ATTRIBUTE_ACCESSORS(UCharacterStatsAttributeSet, Helth)
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData MaxHelth;
    ATTRIBUTE_ACCESSORS(UCharacterStatsAttributeSet, MaxHelth)
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UCharacterStatsAttributeSet, Stamina)
    UPROPERTY(BlueprintReadOnly, Category = "Attributes")
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(UCharacterStatsAttributeSet, MaxStamina)
public:
    UCharacterStatsAttributeSet();
	
};
