// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack_BlackboardBase.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "MyCharacter.h"
#include "AI_01.h"
#include "AI_Controller.h"
UBTT_Attack_BlackboardBase::UBTT_Attack_BlackboardBase()
{
	NodeName = "AtackPlayer";
}

EBTNodeResult::Type UBTT_Attack_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAI_Controller* owner = Cast<AAI_Controller>(OwnerComp.GetAIOwner());
	if (owner == nullptr) {
		return EBTNodeResult::Failed;
	}
	AAI_01* ai = Cast<AAI_01>(owner->GetPawn());
	if (ai) {
		//Call Bluprint Event PlayAnim
		ai->PlayAnim();
	}
	return EBTNodeResult::Succeeded;
}
