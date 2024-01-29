// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "AI_01.h"
#include "AI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AAI_Controller : public AAIController
{
	GENERATED_BODY()
public:
	AAI_Controller();

	//get Blackboard
	UBlackboardComponent* getBlackBoard();
	//is_Patrol
	void setIsPatrol(bool is_patrol);
	bool getIsPatrol();
	//Is_hear_see
	void setIsHearSee(bool is_hera_see);
	bool getIsHearSee();
	//is_attacking
	bool getIsAttacking();
	void setIsAttacking(bool is_under_attack);
protected:
	UBehaviorTreeComponent* BTC;
	UBlackboardComponent* BC;
	//Initialize AI Start Behaiviour and Run Behaivior Tree
	virtual void OnPossess(APawn* myPawn) override;

	virtual FRotator GetControlRotation() const override;
	//Sight Radius
	UPROPERTY(EditAnywhere, Category = AI)
	float AISightRadius = 500.0f;
	//Sight Radius where AI lose focus
	UPROPERTY(EditAnywhere, Category = AI)
	float AILoseSightRadius = 1000.0f;
	//Hear Radius
	UPROPERTY(EditAnywhere, Category = AI)
	float AIHearingRange = 1500.0f;
	//Hear Radius where AI cant hear player
	UPROPERTY(EditAnywhere, Category = AI)
	float LoSHearingRange = 1600.0f;

	//Configs for see and hear player
	class UAISenseConfig_Sight* SenceCongifSight;
	class UAISenseConfig_Hearing* SenceConfigHearing;
	//BlackBoard values
	bool is_Patrol = false;
	bool is_Hear_See = false;
	bool is_Attacking = false;
	//Function to Update Logic When See a Player
	UFUNCTION()
	void OnPawnDetected(AActor* UpdatedActor, FAIStimulus Stimulus);
};
