// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_SetSpeed_BlackboardBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTS_SetSpeed_BlackboardBase : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTS_SetSpeed_BlackboardBase();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
private:
	//Speed property
	UPROPERTY(EditAnywhere)
	float speed;
	
};
