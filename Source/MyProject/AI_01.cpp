// Fill out your copyright notice in the Description page of Project Settings.
#include "AI_01.h"
#include "AI_Controller.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacter.h"
// Sets default values
AAI_01::AAI_01()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BehaviourTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTree"));
	UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	UI->SetupAttachment(RootComponent);
	this->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
}



void AAI_01::Attack(USkeletalMeshComponent* Player, UArrowComponent* top, FName sword_soket)
{
	FHitResult is_hittet;
	FVector start = Player->GetSocketLocation(sword_soket);
	FVector end = top->GetComponentLocation();
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Init(this, 1);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), start, end, 10.f, ObjectTypesArray, false, IgnoredActors, EDrawDebugTrace::None, is_hittet, true);
	if (is_hittet.bBlockingHit) {
		AMyCharacter* OtherActor = Cast<AMyCharacter>(is_hittet.GetActor());
		if (OtherActor) {
			if ((is_hittet.Location - start).Length() <= 500.0f) {
				UGameplayStatics::ApplyDamage(OtherActor, 50, GetController(), this, UDamageType::StaticClass());
			}

		}
	}
}

// Called when the game starts or when spawned
void AAI_01::BeginPlay()
{
	Super::BeginPlay();
	UpdateWidget(helth, maxhelth);

}

float AAI_01::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (can_apply && !is_block) {
		AAI_Controller* controller = Cast<AAI_Controller>(GetController());
		if (helth != 0.0f && helth > 0.0f) {
			helth -= DamageAmount;
			UpdateWidget(helth, maxhelth);
			if (controller) {
				controller->setIsAttacking(true);
				controller->setIsHearSee(false);
				controller->setIsPatrol(false);
				controller->getBlackBoard()->SetValueAsBool(TEXT("is_Patrol"), controller->getIsPatrol());
				controller->getBlackBoard()->SetValueAsBool(TEXT("is_Hear"), controller->getIsHearSee());
				controller->getBlackBoard()->SetValueAsBool(TEXT("is_Attacking"), controller->getIsAttacking());
				controller->getBlackBoard()->SetValueAsObject(TEXT("Player"), DamageCauser);
				//controller->getBlackBoard()->SetValueAsObject(TEXT("AI"), this);
			}
		}
		else {
			this->Destroy();
			controller->getBlackBoard()->ClearValue(TEXT("is_calling_for_help"));
		}
		UE_LOG(LogTemp, Warning, TEXT("HIt Actor %s"), *DamageCauser->GetName());
		DamageTaken();
		return DamageAmount;
	}
	return DamageAmount;
}

UUserWidget* AAI_01::GetWidgetFromWidgetComponent()
{
	return UI->GetUserWidgetObject();
}

// Called every frame
void AAI_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter) {
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(UI->GetComponentLocation(), MyCharacter->getCamera()->GetComponentLocation());
		UI->SetWorldRotation(rot);
	}

}

// Called to bind functionality to input
void AAI_01::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAI_01::SetMaxWalkSpeed(float speed)
{
	this->GetCharacterMovement()->MaxWalkSpeed = speed;
}


