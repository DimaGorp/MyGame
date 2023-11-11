// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"

// Sets default values
AMyCharacter::AMyCharacter():Super()
{
	speed = 700.0f;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 500.0f;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	camera->SetupAttachment(SpringArmComp);

	SpringArmComp->bUsePawnControlRotation = true;
	camera->bUsePawnControlRotation = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* MyInputComponent)
{
	Super::SetupPlayerInputComponent(MyInputComponent);
	MyInputComponent->BindAxis("F_B_Movement", this, &AMyCharacter::F_B_Movement);
	MyInputComponent->BindAxis("R_L_Movement", this, &AMyCharacter::R_L_Movement);
	MyInputComponent->BindAxis("Mouse_Up_Down", this, &AMyCharacter::Mouse_Up_Down);
	MyInputComponent->BindAxis("Mouse_R_L", this, &AMyCharacter::Mouse_R_L);
	MyInputComponent->BindAction("Jump",IE_Pressed , this, &AMyCharacter::Jump);
	MyInputComponent->BindAction("StopJump",IE_Released , this, &AMyCharacter::StopJump);
}
void AMyCharacter::F_B_Movement(float forwardaxis)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, forwardaxis * speed);
}
void AMyCharacter::R_L_Movement(float sideaxis)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, sideaxis * speed);
}

void AMyCharacter::Mouse_Up_Down(float mouseaxis)
{
	AddControllerYawInput(200.f * mouseaxis * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::Mouse_R_L(float mouseaxis)
{
	
	AddControllerPitchInput(200.f * mouseaxis * GetWorld()->GetDeltaSeconds());
}

void AMyCharacter::Jump()
{
	bPressedJump = true;
}
void AMyCharacter::StopJump()
{
	bPressedJump = false;
}

void AMyCharacter::setSpeed()
{
}


