// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Controller.h"
#include "Perception//AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "Perception/AIPerceptionSystem.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"


AAI_Controller::AAI_Controller()
{
	
	//AI Perseption setup
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));
	//SenceConfigSight
	SenceCongifSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SenceCongifSight) {
		SenceCongifSight->SightRadius = AISightRadius;
		SenceCongifSight->LoseSightRadius = AILoseSightRadius;
		SenceCongifSight->PeripheralVisionAngleDegrees = 60.0f;
		SenceCongifSight->SetMaxAge(1);
		SenceCongifSight->DetectionByAffiliation.bDetectEnemies = true;
		SenceCongifSight->DetectionByAffiliation.bDetectFriendlies = false;
		SenceCongifSight->DetectionByAffiliation.bDetectNeutrals = false;
	}
	//SenceConfigHearing
	SenceConfigHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	if (SenceConfigHearing) {
		SenceConfigHearing->HearingRange = AIHearingRange;
		SenceConfigHearing->LoSHearingRange = LoSHearingRange;
		SenceConfigHearing->SetMaxAge(1);
		SenceConfigHearing->DetectionByAffiliation.bDetectEnemies = true;
		SenceConfigHearing->DetectionByAffiliation.bDetectFriendlies = false;
		SenceConfigHearing->DetectionByAffiliation.bDetectNeutrals = false;
	}
	//ConfigureSences
	GetPerceptionComponent()->ConfigureSense(*SenceCongifSight);
	GetPerceptionComponent()->ConfigureSense(*SenceConfigHearing);
	//Binding
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnPawnDetected);
	//BehaviorTree and Blackboard
	BTC = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior tree component"));
	BC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard component"));
}

void AAI_Controller::OnPossess(APawn* myPawn)
{
	Super::OnPossess(myPawn);
	this->is_Patrol = true;
	AAI_01* character = Cast<AAI_01>(myPawn);
	if (character)
	{
		if (character->BehaviourTree->BlackboardAsset) {
			Blackboard->InitializeBlackboard(*(character->BehaviourTree->BlackboardAsset));
			Blackboard->SetValueAsBool(TEXT("is_Patrol"), is_Patrol);
			BTC->StartTree(*character->BehaviourTree);
		}
	}
}

FRotator AAI_Controller::GetControlRotation() const
{
	if(GetPawn() == nullptr){
		return FRotator(0.0f,0.0f,0.0f);
	}
	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
	
}
UBlackboardComponent* AAI_Controller::getBlackBoard()
{
	return Blackboard;
}
void AAI_Controller::setIsPatrol(bool is_patrol)
{
	is_Patrol = is_patrol;
}
bool AAI_Controller::getIsPatrol()
{
	return is_Patrol;
}
void AAI_Controller::setIsHearSee(bool is_hera_see)
{
	 is_Hear_See = is_hera_see;
}
bool AAI_Controller::getIsHearSee()
{
	return is_Hear_See;
}
bool AAI_Controller::getIsAttacking()
{
	return is_Attacking;
}
void AAI_Controller::setIsAttacking(bool is_under_attack)
{
	this->is_Attacking = is_under_attack;
}
void AAI_Controller::OnPawnDetected(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	FAISenseID sightid = SenceCongifSight->GetSenseID();
	FAISenseID hearid = SenceConfigHearing->GetSenseID();
	if (Stimulus.WasSuccessfullySensed()) {
		if (Stimulus.Type == sightid || Stimulus.Type == hearid){
			if (UpdatedActor->ActorHasTag("Player")) {
				this->is_Hear_See = true;
				this->is_Patrol = false;
				Blackboard->SetValueAsBool(TEXT("is_Hear"), is_Hear_See);
				Blackboard->SetValueAsBool(TEXT("is_Patrol"), is_Patrol);
			}
		}
	}
	else {
		this->is_Hear_See = false;
		this->is_Patrol = true;
		Blackboard->SetValueAsBool(TEXT("is_Patrol"), is_Patrol);
		Blackboard->ClearValue(TEXT("is_Hear"));
	}
	
}
