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
	//Get Navigation Mesh
	UNavigationSystemV1* navSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	//Get Player
	ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (MyCharacter) {
		//Get Player Location
		FVector PlayerOrigin = MyCharacter->GetActorLocation();
		//Get AI Location
		FVector AIOrigin = owner->GetPawn()->GetActorLocation();
		//Get the opposite direction of AI face to a player vector
		FVector Rotation = UKismetMathLibrary::FindLookAtRotation(AIOrigin, PlayerOrigin).Vector() * -500.0f;
		//Choose new location from AI location and opposite direction vector
		FVector Sum = AIOrigin + Rotation;
		//Set value as a Vector in BlackBoard Key
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(TEXT("RunLocation"), FVector(Sum.X, Sum.Y, AIOrigin.Z));
	}

	
	return EBTNodeResult::Succeeded;
}