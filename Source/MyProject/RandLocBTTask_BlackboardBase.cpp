// Fill out your copyright notice in the Description page of Project Settings.
#include "RandLocBTTask_BlackboardBase.h"
#include "AI_Controller.h"
#include "NavigationSystem.h"
//#include "AI/Navigation/NavigationTypes.h"


URandLocBTTask_BlackboardBase::URandLocBTTask_BlackboardBase()
{
	NodeName = "RandomLocation";
}

EBTNodeResult::Type URandLocBTTask_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAI_Controller* owner = Cast<AAI_Controller>(OwnerComp.GetAIOwner());
	if (owner == nullptr) {
		return EBTNodeResult::Failed;
	}
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	FVector Origin = owner->GetPawn()->GetActorLocation();
	FNavLocation loc;
	if (navSystem->GetRandomReachablePointInRadius(Origin, 1500, loc)) {
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("RandomLocation"),loc.Location);
	}
	
	return EBTNodeResult::Succeeded;
}
