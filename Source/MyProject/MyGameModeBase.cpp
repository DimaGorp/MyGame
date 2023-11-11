// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"

AMyGameModeBase::AMyGameModeBase() :Super()
{
	ConstructorHelpers::FClassFinder<APawn> MainCharacterPlayer(TEXT("/Game/Bluprints/BP_MyCharacter"));
	DefaultPawnClass = MainCharacterPlayer.Class;
}