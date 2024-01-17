// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.h"
#include "AI_01.generated.h"

UCLASS()
class MYPROJECT_API AAI_01 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_01();
	//Widget Component
	UPROPERTY(EditAnywhere)
	UWidgetComponent* UI;
	UFUNCTION(BlueprintCallable)
	void Attack(USkeletalMeshComponent* Player, UArrowComponent* top, FName sword_soket);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere,Category = Properties)
	float maxhelth = 200.0f;
	UPROPERTY(EditAnywhere, Category = Properties)
	float helth = maxhelth;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Properties)
	bool can_apply = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool is_block = false;
private:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
public:	
	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTree* BehaviourTree;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DamageSystem")
	void DamageTaken();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DamageSystem")
	void PlayAnim();
	//Update Enemy Widget
	UFUNCTION(BlueprintImplementableEvent, Category = "DamageSystem")
	void UpdateWidget(float cur_helth,float max_helth);
	UFUNCTION(BlueprintCallable, Category = "DamageSystem")
	UUserWidget * GetWidgetFromWidgetComponent();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetMaxWalkSpeed(float speed);
};
