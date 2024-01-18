// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Attack_BlackboardBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UBTT_Attack_BlackboardBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_Attack_BlackboardBase();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
};
