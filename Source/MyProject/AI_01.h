// Fill out your copyright notice in the Description page of Project Settings.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AI_01.generated.h"

UCLASS()
class MYPROJECT_API AAI_01 : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_01();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = AI)
	UBehaviorTree* BehaviourTree;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
