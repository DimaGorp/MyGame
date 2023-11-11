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
	~AAI_Controller();
	virtual void OnPossess(APawn* myPawn) override;
	virtual FRotator GetControlRotation() const override;
	UPROPERTY(EditAnywhere,Category = AI)
	float AISightRadius = 1500.0f;
	UPROPERTY(EditAnywhere, Category = AI)
	float AILoseSightRadius = 2000.0f;
	UPROPERTY(EditAnywhere, Category = AI)
	float AIHearingRange = 2500.0f;
	UPROPERTY(EditAnywhere, Category = AI)
	float LoSHearingRange = 3000.0f;

	//MyFunctions
	UBlackboardComponent* getBlackBoard();

	
private:
	class UAISenseConfig_Sight* SenceCongifSight;
	class UAISenseConfig_Hearing* SenceConfigHearing;
	class UAISenseConfig_Damage* SenceConfigDamage;

	UFUNCTION()
	void OnPawnDetected(AActor* UpdatedActor, FAIStimulus Stimulus);
	//BlackBoard varibles
	bool is_Patrol = false;
	bool is_Hear = false;
	bool is_Attacking = false;

	UBehaviorTreeComponent* BTC;
	UBlackboardComponent* BC;
};
