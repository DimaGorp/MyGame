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
	virtual void OnPossess(APawn* myPawn) override;
	virtual FRotator GetControlRotation() const override;
	UPROPERTY(EditAnywhere,Category = AI)
	float AISightRadius = 500.0f;
	UPROPERTY(EditAnywhere, Category = AI)
	float AILoseSightRadius = 1000.0f;
	UPROPERTY(EditAnywhere, Category = AI)
	float AIHearingRange = 1500.0f;
	UPROPERTY(EditAnywhere, Category = AI)
	float LoSHearingRange = 1600.0f;

	//MyFunctions
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
private:
	class UAISenseConfig_Sight* SenceCongifSight;
	class UAISenseConfig_Hearing* SenceConfigHearing;
	UFUNCTION()
	void OnPawnDetected(AActor* UpdatedActor, FAIStimulus Stimulus);
	//BlackBoard varibles
	bool is_Patrol = false;
	bool is_Hear_See = false;
	bool is_Attacking = false;


	UBehaviorTreeComponent* BTC;
	UBlackboardComponent* BC;
};
