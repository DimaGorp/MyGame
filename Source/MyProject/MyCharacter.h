// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/ArrowComponent.h"
#include "MotionWarpingComponent.h"
#include "AbilitySystemInterface.h"
#include "GAS/CharacterAttributeSet.h"
#include "Abilities/GameplayAbility.h"
#include "MyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* MyInputComponent) override;
	//Basic interactions with enviournment
	//movement
	void F_B_Movement(float forwardaxis);
	void R_L_Movement(float sideaxis);
	//camera rotation 
	void Mouse_Up_Down(float mouseaxis);
	void Mouse_R_L(float mouseaxis);
	//jumping
	void Jump();
	void StopJump();
	//sprinting
	void StartSprint();
	void StopSprint();
	//Crounching
	void StartCrounching();
	void StopCrounching();
	//MakingNoise
	UFUNCTION(BlueprintCallable)
	void MakingNoise(float volume);
	//Healing
	UFUNCTION(BlueprintCallable)
	bool Heal();
	//Get Camera
	UCameraComponent* getCamera();
	//decrease amout of enemies
	UFUNCTION(BlueprintCallable)
	void decreasenemycount();
	//Attacking
	UFUNCTION(BlueprintCallable)
	void Attack(USkeletalMeshComponent *Player,UArrowComponent *top, FName sword_soket);
	//Method to be implmented in bluprints(no implmentation)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DamageSystem")
	void DamageTaken();
	//Update UI
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DamageSystem")
	void UpdateUI(float cur_helth,float max_helth);
	//GrandPlayer and Ability
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "GAS")
	void Grand();
	//Warp at Target
	UFUNCTION(BlueprintCallable)
	void StartMWarping(FName victim, AActor* target);
	UFUNCTION(BlueprintCallable)
	void StartMWarpingComponent(FName victim, USceneComponent* target);
	UFUNCTION(BlueprintCallable)
	void StopMWarping(FName victim);
	//TakeDown
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable,  Category = "DamageSystem")
	void TakeDown(AActor* target,FVector LOcation,FRotator Rotation);
	//TargetAtBack
	UFUNCTION(BlueprintCallable)
	void StartCheckIsBack();
	//PLayer UI Update enemy count icon
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void UpdateEnemyCountUI(int enemies);
private:
	//Properties
	//Speed
	float speed;
	//Helth
	UPROPERTY(EditAnywhere, Category = Properties)
	float maxhelth = 100.0f;
	UPROPERTY(EditAnywhere, Category = Properties)
	float helth = maxhelth;
	//Game Over Widget
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> GameOverWidget;
	UUserWidget* GameOver;
	//Win Widget
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> WinWidget;
	UUserWidget* Win;
	//Player UI Widget
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> WidgetUI;
	//AI stimuli source
	UAIPerceptionStimuliSourceComponent* source_hear;
	//Recive damage from Enemy
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	//Target Lock objects
	void StartTarget();
	void CheckWinning();
protected:
	//Components in Viewport 
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMotionWarpingComponent* MotionWarpComponent;
	//Player UI object reference
	UPROPERTY(BlueprintReadWrite, Category = UI)
	UUserWidget* UI;

	//BottleOfPotionHealing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	int count_of_potions = 0;

	//EnemiesAmount
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	int count_of_enemies = 0;


	//IS weapon equiped
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
	bool is_weapon_equiped;
	//Variable to take only one damage by sword trace
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool can_apply = true;
	//Block variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool is_block = false;
	//bool variable if object is under targetting 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool is_targeting = false;
	//Object that is targetting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Targets)
	AActor* target_object;
	//Is Player Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool is_Attacking = false;
	//Is PLayer Crouch
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool is_Crouch = false;
//Gameplay Ability System
protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="GAS")
	class UAbilitySystemComponent* GA_Component;
	virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override{
		return GA_Component;
	}
	UFUNCTION(BlueprintCallable, Category = "GAS")
	void InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 AbilityLevel);
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
    const class UCharacterAttributeSet* AttributeSet;

};
