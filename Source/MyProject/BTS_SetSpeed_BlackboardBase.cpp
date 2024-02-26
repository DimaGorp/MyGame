// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_SetSpeed_BlackboardBase.h"
#include "AI_Controller.h"
#include "AI_01.h"
UBTS_SetSpeed_BlackboardBase::UBTS_SetSpeed_BlackboardBase()
{
	NodeName = "SetSpeed";
}

void UBTS_SetSpeed_BlackboardBase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAI_Controller *controller = Cast<AAI_Controller>(OwnerComp.GetAIOwner());
	if (controller) {
		AAI_01 *pawn = Cast<AAI_01>(controller->GetPawn());
		if (pawn)
		{
			//Set AI speed
			pawn->SetMaxWalkSpeed(speed);
		}
	}
}
