// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PlayerWidget.h"
#include "Blueprint/UserWidget.h"
#include "Components/ArrowComponent.h"
#include "MotionWarpingComponent.h"
#include "MyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyCharacter : public ACharacter
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
	//Get Camera
	UCameraComponent* getCamera();
	//Attacking
	UFUNCTION(BlueprintCallable)
	void Attack(USkeletalMeshComponent *Player,UArrowComponent *top, FName sword_soket);
	//Method to be implmented in bluprints(no implmentation)
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "DamageSystem")
	void DamageTaken();
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
private:
	//Properties
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
	//Player UI
	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UPlayerWidget> WidgetUI;
	UPlayerWidget* UI;
	//AI stimuli source
	UAIPerceptionStimuliSourceComponent* source_hear;
	//Recive damage from Enemy
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void BeginPlay() override;
	//Target Lock objects
	void StartTarget();
protected:
	//Components in Viewport 
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* camera;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UMotionWarpingComponent* MotionWarpComponent;
	//IS weapon equiped
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Variables)
	bool is_weapon_equiped;
	//Variable to take onlu one damage by sphere trace
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool can_apply = true;
	//Block variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool is_block = false;
	//Object that is target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool is_targeting = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Targets)
	AActor* target_object;
	//Is Player Attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool is_Attacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
	bool is_Crouch = false;

};
