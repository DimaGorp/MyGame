// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "Perception/AISense_Hearing.h"
#include "AI_01.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "MotionWarpingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <iostream>
#include <cmath>
// Sets default values
AMyCharacter::AMyCharacter():Super()
{
	speed = 0.0f;
 	//Components in ViewPort
	PrimaryActorTick.bCanEverTick = false;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 500.0f;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	camera->SetupAttachment(SpringArmComp);

	MotionWarpComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpComponent"));

	SpringArmComp->bUsePawnControlRotation = true;
	camera->bUsePawnControlRotation = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	//Stimulus Hearing
	source_hear = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliSourceComponent"));
	source_hear->RegisterForSense(UAISense_Hearing::StaticClass());
	this->GetCharacterMovement()->MaxWalkSpeed = speed + 250.0f;


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
	MyInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::StartSprint);
	MyInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::StopSprint);
	MyInputComponent->BindAction("Target", IE_Pressed, this, &AMyCharacter::StartTarget);
	MyInputComponent->BindAction("Crounch", IE_Pressed, this, &AMyCharacter::StartCrounching);
	MyInputComponent->BindAction("Crounch", IE_Released, this, &AMyCharacter::StopCrounching);
}
void AMyCharacter::F_B_Movement(float forwardaxis)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
	AddMovementInput(Direction, forwardaxis);
}
void AMyCharacter::R_L_Movement(float sideaxis)
{
	FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, sideaxis);
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

void AMyCharacter::StartSprint()
{
	speed = 600.0f;
	this->GetCharacterMovement()->MaxWalkSpeed = speed;
}

void AMyCharacter::StopSprint()
{
	speed = 250.0f;
	this->GetCharacterMovement()->MaxWalkSpeed = speed;
}

void AMyCharacter::StartCrounching()
{
	Crouch();
	is_Crouch = true;
}

void AMyCharacter::StopCrounching()
{
	UnCrouch();
	is_Crouch = false;
}

void AMyCharacter::MakingNoise(float volume)
{
	MakeNoise(volume, this, GetActorLocation());
}

bool AMyCharacter::Heal()
{
	if (count_of_potions > 0) {
		if (helth != maxhelth) {
			helth += (maxhelth - helth);
			UpdateUI(helth, maxhelth);
			count_of_potions--;
			return true;
		}
	}
	return false;
}

void AMyCharacter::Attack(USkeletalMeshComponent* Player,UArrowComponent* top,FName sword_soket)
{
	
	if (is_weapon_equiped) {
		FHitResult is_hittet;
		FVector start = Player->GetSocketLocation(sword_soket);
		FVector end = top->GetComponentLocation();
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Init(this,1);
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
		ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), start, end, 50.f, ObjectTypesArray, false, IgnoredActors, EDrawDebugTrace::None, is_hittet, true);
		if (is_hittet.bBlockingHit){
			AAI_01* OtherActor = Cast<AAI_01>(is_hittet.GetActor());
			if (OtherActor) {
				UGameplayStatics::ApplyDamage(OtherActor, 50, GetController(), this, UDamageType::StaticClass());

			}
		}
	}

}


UCameraComponent* AMyCharacter::getCamera()
{
	return camera;
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (can_apply && !is_block) {
		if (helth > 0.0f && helth != 0.0f) {
			helth -= DamageAmount;
			//UE_LOG(LogTemp, Warning, TEXT("Helth is %s"), *FString::SanitizeFloat(helth));
			UpdateUI(helth, maxhelth);
		}
		else {
			GameOver = CreateWidget<UUserWidget>(GetWorld(), GameOverWidget);
			APlayerController* MyController = GetWorld()->GetFirstPlayerController();
			if (MyController) {
				MyController->bShowMouseCursor = true;
				MyController->SetInputMode(FInputModeUIOnly());
			}
			GameOver->AddToViewport(0);
			this->Destroy();
		}
		DamageTaken();
		return DamageAmount;
	}
	return DamageAmount;
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	UI = CreateWidget<UUserWidget>(GetWorld(), WidgetUI);
	UpdateUI(helth, maxhelth);
	UI->AddToViewport();
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	if (MyController) {
		MyController->bShowMouseCursor = false;
		MyController->SetInputMode(FInputModeGameOnly());
	}
}


void AMyCharacter::StartTarget()
{
	if (is_targeting) {
		is_targeting = false;
		target_object = nullptr;
	}
	else {
		FHitResult is_hittet;
		FVector start = camera->GetComponentLocation();
		FVector end = start + (camera->GetForwardVector() * 2000);
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Init(this, 1);
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
		ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), start, end, 100.f, ObjectTypesArray, false, IgnoredActors, EDrawDebugTrace::ForDuration, is_hittet, true);
		if (is_hittet.bBlockingHit) {
			AAI_01* OtherActor = Cast<AAI_01>(is_hittet.GetActor());
			if (OtherActor){
				target_object = OtherActor;
			}
			is_targeting = true;
		}
	}
}

void AMyCharacter::StartCheckIsBack()
{
	FHitResult is_hittet;
	FVector start = camera->GetComponentLocation();
	FVector end = start + (camera->GetForwardVector() * 2000);
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Init(this, 1);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), start, end, 50.f, ObjectTypesArray, false, IgnoredActors, EDrawDebugTrace::ForDuration, is_hittet, true);
	if (is_hittet.bBlockingHit) {
		AAI_01* OtherActor = Cast<AAI_01>(is_hittet.GetActor());
		if (OtherActor) {
			FVector Location = OtherActor->GetActorLocation();
			FRotator Rotation = OtherActor->GetActorRotation();
			FVector RotForward = FRotationMatrix(OtherActor->GetActorRotation()).GetScaledAxis(EAxis::X);
			FVector RotRight = FRotationMatrix(OtherActor->GetActorRotation()).GetScaledAxis(EAxis::Y);
			RotForward *= 65.0f;
			RotRight *= 20.0f;
			Location -= FVector(RotForward.X, RotForward.Y, 90.0f);
			Location -= RotRight;
			if ((GetActorLocation() - OtherActor->GetActorLocation()).Length() <= 300.0f){
				if(UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), Location),GetActorForwardVector()) <=45.0f){
				//if (UKismetMathLibrary::InverseTransformLocation(OtherActor->GetActorTransform(), Location).Y <= 0.0f) {
					TakeDown(OtherActor, Location, Rotation);
				}
			}

		}
	}
}


void AMyCharacter::StartMWarping(FName victim, AActor* target)
{
	AAI_01* OtherActor = Cast<AAI_01>(target);
	if (OtherActor) {
		MotionWarpComponent->AddOrUpdateWarpTargetFromLocationAndRotation(victim, OtherActor->GetActorLocation(),GetActorRotation());
	}
}

void AMyCharacter::StartMWarpingComponent(FName victim, USceneComponent* target)
{
	AAI_01* OtherActor = Cast<AAI_01>(target->GetOwner());
	if (OtherActor) {
		if (target) {
			MotionWarpComponent->AddOrUpdateWarpTargetFromLocationAndRotation(victim, target->GetComponentLocation(), GetActorRotation());
		}
	}
}

void AMyCharacter::StopMWarping(FName victim)
{
	MotionWarpComponent->RemoveWarpTarget(victim);
}

