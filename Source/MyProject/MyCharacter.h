// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyCharacter.generated.h"

UCLASS()
class MYPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();
	void setSpeed();
	virtual void SetupPlayerInputComponent(class UInputComponent* MyInputComponent) override;
	void F_B_Movement(float forwardaxis);
	void R_L_Movement(float sideaxis);
	void Mouse_Up_Down(float mouseaxis);
	void Mouse_R_L(float mouseaxis);
	void Jump();
	void StopJump();
	
	
private:
	float speed;
protected:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* camera;

};
