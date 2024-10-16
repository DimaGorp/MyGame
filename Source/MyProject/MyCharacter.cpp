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
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemComponent.h"
#include "GAS/CharacterAttributeSet.h"
#include <iostream>
#include <cmath>
// Sets default values
AMyCharacter::AMyCharacter():Super()
{
	speed = 0.0f;
 	//Components in ViewPort
	PrimaryActorTick.bCanEverTick = true;
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

	//Gameplay Ability System Component
	GA_Component = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("GameplayAbilitySystem Component"));


}
//Binding
void AMyCharacter::SetupPlayerInputComponent(class UInputComponent* MyInputComponent)
{
	Super::SetupPlayerInputComponent(MyInputComponent);
	MyInputComponent->BindAxis("F_B_Movement", this, &AMyCharacter::F_B_Movement);
	MyInputComponent->BindAxis("R_L_Movement", this, &AMyCharacter::R_L_Movement);
	MyInputComponent->BindAxis("Mouse_Up_Down", this, &AMyCharacter::Mouse_Up_Down);
	MyInputComponent->BindAxis("Mouse_R_L", this, &AMyCharacter::Mouse_R_L);
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
	//check amount of potions
	if (count_of_potions > 0) {
		if (helth != maxhelth) {
			helth += (maxhelth - helth);
			//add helth 
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
		//get location of sword socket
		FVector start = Player->GetSocketLocation(sword_soket);
		//get location of arrow on a sword
		FVector end = top->GetComponentLocation();
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Init(this,1);
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
		ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		//Sphere trace by sword
		UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), start, end, 50.f, ObjectTypesArray, false, IgnoredActors, EDrawDebugTrace::None, is_hittet, true);
		if (is_hittet.bBlockingHit){
			if(is_Attacking){
				AAI_01* OtherActor = Cast<AAI_01>(is_hittet.GetActor());
				if (OtherActor) {
					DealDamage(OtherActor,is_hittet.Location);
					is_Attacking = false;
					//UGameplayStatics::ApplyDamage(OtherActor, 50, GetController(), this, UDamageType::StaticClass());
				}
			}
		}
	}

}


UCameraComponent* AMyCharacter::getCamera()
{
	return camera;
}


void AMyCharacter::decreasenemycount()
{
	count_of_enemies--;
	target_object = nullptr;
	UpdateEnemyCountUI(count_of_enemies);
	//Check if no enemies is on map
	CheckWinning();
}

void AMyCharacter::HealthChanged(const FOnAttributeChangeData& Data)
{
	if (!is_block) {
		
		if (Data.NewValue < Data.OldValue) {
			//Call Bluprint method "DamageTake"
			DamageTaken();
			
		}
		//Dead
		if (GetHelth() == 0.0f) {
			//Remove Player UI
			UI->RemoveFromViewport();
			GameOver = CreateWidget<UUserWidget>(GetWorld(), GameOverWidget);
			APlayerController* MyController = GetWorld()->GetFirstPlayerController();
			if (MyController) {
				MyController->bShowMouseCursor = true;
				MyController->SetInputMode(FInputModeUIOnly());
			}
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			GameOver->AddToViewport(0);
			this->Destroy();
			return;
		}
		else {
			UpdateUI(GetHelth(), GetMaxHelth());
			return;
		}
	}
}
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(GA_Component)) {
		AttributeSet = GA_Component->GetSet<UCharacterAttributeSet>();
		// Attribute change callbacks
		GA_Component->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHelthAttribute()).AddUObject(this, &AMyCharacter::HealthChanged);
	}
	//Create Player UI
	UI = CreateWidget<UUserWidget>(GetWorld(), WidgetUI);
	UpdateUI(GetHelth(), GetMaxHelth());
	UI->AddToViewport();
	//Show mouse cursor
	APlayerController* MyController = GetWorld()->GetFirstPlayerController();
	if (MyController) {
		MyController->bShowMouseCursor = false;
		MyController->SetInputMode(FInputModeGameOnly());
	}
	//Found AI`s on map
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAI_01::StaticClass(), FoundActors);
	//set count of enemies
	count_of_enemies = FoundActors.Num();
	//Set amout of enemies at map in Player UI icon
	UpdateEnemyCountUI(count_of_enemies);

	GrandAbility();
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (target_object) {
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target_object->GetActorLocation());
		GetController()->SetControlRotation(FRotator(rot.Pitch - 25.0f,rot.Yaw,rot.Roll));
	}
	UpdateUI(GetHelth(), GetMaxHelth());
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
		UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), start, end, 200.f, ObjectTypesArray, false, IgnoredActors, EDrawDebugTrace::None, is_hittet, true);
		if (is_hittet.bBlockingHit) {
			AAI_01* OtherActor = Cast<AAI_01>(is_hittet.GetActor());
			if (OtherActor){
				target_object = OtherActor;
			}
			is_targeting = true;
		}
	}
}

void AMyCharacter::CheckWinning()
{
	if (count_of_enemies == 0) {
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		//Remove PlayerUI
		UI->RemoveFromViewport();
		//Show Winner UI
		Win = CreateWidget<UUserWidget>(GetWorld(), WinWidget);
		APlayerController* MyController = GetWorld()->GetFirstPlayerController();
		if (MyController) {
			MyController->bShowMouseCursor = true;
			MyController->SetInputMode(FInputModeUIOnly());
		}
		Win->AddToViewport(0);
	}
}

void AMyCharacter::InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 InputID)
{
	if (GA_Component) {
		if (HasAuthority() && AbilityToGet) {
			GA_Component->GiveAbility(FGameplayAbilitySpec(AbilityToGet, 0, InputID));
		}
		GA_Component->InitAbilityActorInfo(this, this);
	}
}


void AMyCharacter::StartCheckIsBack()
{
	FHitResult is_hittet;
	FVector start = camera->GetComponentLocation();
	FVector end = start + (camera->GetForwardVector() * 2000);
	//Ignored actors
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Init(this, 1);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	//Sphere Trace
	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), start, end, 200.f, ObjectTypesArray, false, IgnoredActors, EDrawDebugTrace::None, is_hittet, true);
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
			//Check the distance to an enemy
			if ((GetActorLocation() - OtherActor->GetActorLocation()).Length() <= 300.0f){
				//Check if the player is behind an enemy
				if(UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), Location),GetActorForwardVector()) <=45.0f){
					TakeDown(OtherActor, Location, Rotation);
				}
			}

		}
	}
}

float AMyCharacter::GetHelth() const
{
	return AttributeSet->GetHelth();
}

float AMyCharacter::GetMaxHelth() const
{
	return AttributeSet->GetMaxHelth();
}

float AMyCharacter::GetStamina() const
{
	return AttributeSet->GetStamina();
}

float AMyCharacter::GetMaxStamina() const
{
	return AttributeSet->GetMaxStamina();
}


void AMyCharacter::StartMWarping(FName victim, AActor* target)
{
	AAI_01* OtherActor = Cast<AAI_01>(target);
	if (OtherActor) {
		MotionWarpComponent->AddOrUpdateWarpTargetFromLocationAndRotation(victim, OtherActor->GetActorLocation(), OtherActor->GetActorRotation());
	}
}

void AMyCharacter::StartMWarpingComponent(FName victim, USceneComponent* target)
{
	AAI_01* OtherActor = Cast<AAI_01>(target->GetOwner());
	if (OtherActor) {
		if (target) {
			MotionWarpComponent->AddOrUpdateWarpTargetFromLocationAndRotation(victim, target->GetComponentLocation(), target->GetComponentRotation());
		}
	}
}

void AMyCharacter::StopMWarping(FName victim)
{
	MotionWarpComponent->RemoveWarpTarget(victim);
}

