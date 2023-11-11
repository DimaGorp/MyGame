// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTRunAway_BlackboardBase.h"
#include "AI_Controller.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"


UBTTRunAway_BlackboardBase::UBTTRunAway_BlackboardBase()
{
	NodeName = "RunAway";
}

EBTNodeResult::Type UBTTRunAway_BlackboardBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAI_Controller* owner = Cast<AAI_Controller>(OwnerComp.GetAIOwner());
	if (owner == nullptr) {
		return EBTNodeResult::Failed;
	}
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector PlayerOrigin = MyCharacter->GetActorLocation();
	FVector AIOrigin = owner->GetPawn()->GetActorLocation();
	FVector Rotation = UKismetMathLibrary::FindLookAtRotation(AIOrigin, PlayerOrigin).Vector() * -500.0f;
	FVector Sum = AIOrigin + Rotation;
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("RunLocation"),FVector(Sum.X,Sum.Y,AIOrigin.Z));

	delete owner;
	delete navSystem;
	delete MyCharacter;
	return EBTNodeResult::Succeeded;
}