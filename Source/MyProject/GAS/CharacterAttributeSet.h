// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "CharacterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName,PropertyName)\
        GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName)\
        GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)\
        GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)\
        GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class MYPROJECT_API UCharacterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
    UCharacterAttributeSet();

    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue);

public:
 

    //Helth
    UPROPERTY(BlueprintReadOnly, Category = "Helth")
    FGameplayAttributeData Helth;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Helth)

    UPROPERTY(BlueprintReadOnly, Category = "Helth")
    FGameplayAttributeData MaxHelth;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHelth)

    //Stamina
    UPROPERTY(BlueprintReadOnly, Category = "Stamina")
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Stamina)

    UPROPERTY(BlueprintReadOnly, Category = "Stamina")
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxStamina)
    
    //Damage 
    UPROPERTY(BlueprintReadOnly, Category = "Damage")
    FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Damage)
	
};
