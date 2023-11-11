// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "RandLocBTTask_BlackboardBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API URandLocBTTask_BlackboardBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	URandLocBTTask_BlackboardBase();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
