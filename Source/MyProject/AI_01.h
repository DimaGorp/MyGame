// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystemInterface.h"
#include "GAS/CharacterAttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "AI_01.generated.h"

UCLASS()
class MYPROJECT_API AAI_01 : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAI_01();

	//SetSpeed
	void SetMaxWalkSpeed(float speed);

	//Bluprint RecieveDamage
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DamageSystem")
	void DamageTaken();

	//Play Attack Animation 
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DamageSystem")
	void PlayAnim();

	//Update Enemy Widget UI
	UFUNCTION(BlueprintImplementableEvent, Category = "DamageSystem")
	void UpdateWidget(float cur_helth, float max_helth);

	//Get Widget from WidgetComponent
	UFUNCTION(BlueprintCallable, Category = "DamageSystem")
	UUserWidget* GetWidgetFromWidgetComponent();

	//Sword Trace
	UFUNCTION(BlueprintCallable)
	void Attack(USkeletalMeshComponent* Player, UArrowComponent* top, FName sword_soket);
	UBehaviorTree* getBehaviourTree();
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void Grand();
	//Bluprint RecieveDamage
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS")
	void DealDamage();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//AttributeSet Getters
	UFUNCTION(BlueprintCallable, Category = "GAS")
	float GetHelth() const;
	UFUNCTION(BlueprintCallable, Category = "GAS")
	float GetMaxHelth() const;
	UFUNCTION(BlueprintCallable, Category = "GAS")
	float GetStamina() const;
	UFUNCTION(BlueprintCallable, Category = "GAS")
	float GetMaxStamina() const;


private:
	//ReceiveDamage
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Binding
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;	
protected:
	//Enemy UI Widget Component
	UPROPERTY(EditAnywhere)
	UWidgetComponent* UI;
	//BehaviorTree
	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTree* BehaviourTree;
	//Properties
	//Helth
	UPROPERTY(EditAnywhere, Category = Properties)
	float maxhelth = 200.0f;
	UPROPERTY(EditAnywhere, Category = Properties)
	float helth = maxhelth;
	//Can Take Damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool can_apply = true;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	class UAbilitySystemComponent* GA_Component;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override {
		return GA_Component;
	}
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
	const class UCharacterAttributeSet* AttributeSet;

	UFUNCTION(BlueprintCallable, Category = "GAS")
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 InputID);
protected:
	//Delegated
	FDelegateHandle HealthChangedDelegateHandle;
	// Attribute changed callbacks
	void HealthChanged(const FOnAttributeChangeData& Data);
};
