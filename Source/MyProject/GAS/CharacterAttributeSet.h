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
    //AtributeChanges
    virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

    //When Effect is Applied to Attribute set
    virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    //Helth
    UPROPERTY(BlueprintReadOnly, Category = "Helth", ReplicatedUsing = OnRep_Helth)
    FGameplayAttributeData Helth;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Helth)

    UPROPERTY(BlueprintReadOnly, Category = "Helth", ReplicatedUsing = OnRep_HelthMax)
    FGameplayAttributeData MaxHelth;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxHelth)

    //Stamina
    UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_Stamina)
    FGameplayAttributeData Stamina;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Stamina)

    UPROPERTY(BlueprintReadOnly, Category = "Stamina", ReplicatedUsing = OnRep_StaminaMax)
    FGameplayAttributeData MaxStamina;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, MaxStamina)
    
    //Damage 
    UPROPERTY(BlueprintReadOnly, Category = "Damage")
    FGameplayAttributeData Damage;
    ATTRIBUTE_ACCESSORS(UCharacterAttributeSet, Damage)
protected:
    //Replicated Functions
    UFUNCTION()
    void OnRep_Helth(const FGameplayAttributeData& OldHelth);
    UFUNCTION()
    void OnRep_HelthMax(const FGameplayAttributeData& OldHelthMax);
    UFUNCTION()
    void OnRep_Stamina(const FGameplayAttributeData& OldStamina);
    UFUNCTION()
    void OnRep_StaminaMax(const FGameplayAttributeData& OldStaminaMax);

	
};
