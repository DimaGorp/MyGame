// Fill out your copyright notice in the Description page of Project Settings.
#include "AI_01.h"

// Sets default values
AAI_01::AAI_01()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void AAI_01::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAI_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAI_01::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


