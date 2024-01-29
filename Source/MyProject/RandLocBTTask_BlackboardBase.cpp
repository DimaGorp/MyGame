// Fill out your copyright notice in the Description page of Project Settings.
#include "RandLocBTTask_BlackboardBase.h"
#include "AI_Controller.h"
#include "NavigationSystem.h"


URandLocBTTask_BlackboardBase::URandLocBTTask_BlackboardBase()
{
	//Node name in Behavior Tree
	NodeName = "RandomLocation";
}

EBTNodeResult::Type URandLocBTTask_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAI_Controller* owner = Cast<AAI_Controller>(OwnerComp.GetAIOwner());
	if (owner == nullptr) {
		return EBTNodeResult::Failed;
	}
	//Get Navigation Mesh
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	//Get Location of AI
	FVector Origin = owner->GetPawn()->GetActorLocation();
	FNavLocation loc;
	//Choose location across AI on Nvigation Mesh
	if (navSystem->GetRandomReachablePointInRadius(Origin, 1500, loc)) {
		//Set location in Blackboard key value as Vector
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("RandomLocation"),loc.Location);
	}
	
	return EBTNodeResult::Succeeded;
}
