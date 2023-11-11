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
	SenceCongifSight->SightRadius = AISightRadius;
	SenceCongifSight->LoseSightRadius = AILoseSightRadius;
	SenceCongifSight->PeripheralVisionAngleDegrees = 60.0f;
	SenceCongifSight->SetMaxAge(1);
	SenceCongifSight->DetectionByAffiliation.bDetectEnemies = true;
	SenceCongifSight->DetectionByAffiliation.bDetectFriendlies = true;
	SenceCongifSight->DetectionByAffiliation.bDetectNeutrals = true;
	//SenceConfigHearing
	SenceConfigHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("Hearing Config"));
	SenceConfigHearing->HearingRange = AIHearingRange;
	SenceConfigHearing->LoSHearingRange = LoSHearingRange;
	SenceConfigHearing->SetMaxAge(1);
	SenceConfigHearing->DetectionByAffiliation.bDetectEnemies = true;
	SenceConfigHearing->DetectionByAffiliation.bDetectFriendlies = true;
	SenceConfigHearing->DetectionByAffiliation.bDetectNeutrals = true;
	//SenceConfigDamage
	SenceConfigDamage = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("Damage Config"));
	SenceConfigDamage->SetMaxAge(1);
	//ConfigureSences
	GetPerceptionComponent()->ConfigureSense(*SenceCongifSight);
	GetPerceptionComponent()->ConfigureSense(*SenceConfigHearing);
	GetPerceptionComponent()->ConfigureSense(*SenceConfigDamage);
	//Binding
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAI_Controller::OnPawnDetected);
	//BehaviorTree and Blackboard
	BTC = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behavior tree component"));
	BC = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoard component"));

}
AAI_Controller::~AAI_Controller()
{
	delete SenceCongifSight;
	delete SenceConfigHearing;
	delete SenceConfigDamage;
	delete BTC;
	delete BC;
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
	delete character;
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
void AAI_Controller::OnPawnDetected(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	FAISenseID sightid = SenceCongifSight->GetSenseID();
	FAISenseID hearid = SenceConfigHearing->GetSenseID();
	FAISenseID damageid = SenceConfigDamage->GetSenseID();
	if (Stimulus.Type == sightid || Stimulus.Type == hearid) {
		if (Stimulus.WasSuccessfullySensed()) {
			this->is_Hear = true;
			this->is_Patrol = false;
			Blackboard->SetValueAsBool(TEXT("is_Patrol"), is_Patrol);
			Blackboard->SetValueAsBool(TEXT("is_Hear"), is_Hear);
		}
		else {
			this->is_Hear = false;
			this->is_Patrol = true;
			Blackboard->ClearValue(TEXT("is_Patrol"));
			Blackboard->ClearValue(TEXT("is_Hear"));
		}
	}
	else if (Stimulus.Type == damageid){
		ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		if (Stimulus.WasSuccessfullySensed()) {
			this->is_Attacking = true;
			Blackboard->SetValueAsBool(TEXT("is_Attacking"), is_Attacking);
			Blackboard->SetValueAsObject(TEXT("Player"), MyCharacter);
		}
		else {
			this->is_Attacking = false;
			Blackboard->ClearValue(TEXT("is_Attacking"));
			Blackboard->ClearValue(TEXT("Player"));
		}
		delete MyCharacter;

	}
}
