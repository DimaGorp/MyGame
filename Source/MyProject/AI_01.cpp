// Fill out your copyright notice in the Description page of Project Settings.
#include "AI_01.h"
#include "AI_Controller.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "GAS/CharacterAttributeSet.h"
#include "MyCharacter.h"
// Sets default values
AAI_01::AAI_01()
{
	PrimaryActorTick.bCanEverTick = true;
	//Create BehaviourTree
	BehaviourTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTree"));
	//Create ViewPort components
	UI = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	UI->SetupAttachment(RootComponent);
	//Set Begin speed
	this->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	//Gameplay Ability System Component
	GA_Component = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("GameplayAbilitySystem Component"));

}



void AAI_01::Attack(USkeletalMeshComponent* Player, UArrowComponent* top, FName sword_soket)
{
	FHitResult is_hittet;
	//get start and end location of sword
	FVector start = Player->GetSocketLocation(sword_soket);
	FVector end = top->GetComponentLocation();
	//Objects to ignore tracing
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Init(this, 1);
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	//Sphere Trace
	UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), start, end, 10.f, ObjectTypesArray, false, IgnoredActors, EDrawDebugTrace::None, is_hittet, true);
	if (is_hittet.bBlockingHit) {
		AMyCharacter* OtherActor = Cast<AMyCharacter>(is_hittet.GetActor());
		if (OtherActor) {
			UGameplayStatics::ApplyDamage(OtherActor, 50, GetController(), this, UDamageType::StaticClass());
		}
	}
}

UBehaviorTree* AAI_01::getBehaviourTree()
{
	return BehaviourTree;
}

// Called when the game starts or when spawned
void AAI_01::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(GA_Component)) {
		AttributeSet = GA_Component->GetSet<UCharacterAttributeSet>();
	}
	if (GA_Component)
	{
		// Attribute change callbacks
		HealthChangedDelegateHandle = GA_Component->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHelthAttribute()).AddUObject(this, &AAI_01::HealthChanged);
	}
	//Set helth into Enemy UI
	UpdateWidget(GetHelth(), GetMaxHelth());
	Grand();

}

float AAI_01::GetHelth() const
{
	return AttributeSet->GetHelth();
}

float AAI_01::GetMaxHelth() const
{
	return AttributeSet->GetMaxHelth();
}

float AAI_01::GetStamina() const
{
	return AttributeSet->GetStamina();
}

float AAI_01::GetMaxStamina() const
{
	return AttributeSet->GetMaxStamina();
}

float AAI_01::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (can_apply) {
		AAI_Controller* controller = Cast<AAI_Controller>(GetController());
		if (GetHelth() != 0.0f && GetHelth() > 0.0f) {
			//helth -= DamageAmount;
			UpdateWidget(GetHelth(), GetMaxHelth());
			if (controller) {
				//set Attack bool variable
				controller->setIsAttacking(true);
				//set variable for AIPerception
				controller->setIsHearSee(false);
				//Patrolling boll variable
				controller->setIsPatrol(false);
				//Write varibles in blackboard
				controller->getBlackBoard()->SetValueAsBool(TEXT("is_Patrol"), controller->getIsPatrol());
				controller->getBlackBoard()->SetValueAsBool(TEXT("is_Hear"), controller->getIsHearSee());
				controller->getBlackBoard()->SetValueAsBool(TEXT("is_Attacking"), controller->getIsAttacking());
				controller->getBlackBoard()->SetValueAsObject(TEXT("Player"), DamageCauser);
			}
		}
		else {
			AMyCharacter* player = Cast<AMyCharacter>(DamageCauser);
			this->Destroy();
			//Change Enemies amout
			player->decreasenemycount();
		}
		//Call Bluprint Event after Recieve Damage
		DamageTaken();
		return DamageAmount;
	}
	return DamageAmount;
}
//Get UI from WidgetComponent
UUserWidget* AAI_01::GetWidgetFromWidgetComponent()
{
	return UI->GetUserWidgetObject();
}

//Event Tick
void AAI_01::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (MyCharacter) {
		FRotator rot = UKismetMathLibrary::FindLookAtRotation(UI->GetComponentLocation(), MyCharacter->getCamera()->GetComponentLocation());
		UI->SetWorldRotation(rot);
	}
	DealDamage();

}

//Binding
void AAI_01::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void AAI_01::InitializeAbility(TSubclassOf<UGameplayAbility> AbilityToGet, int32 InputID)
{
	if (GA_Component) {
		if (HasAuthority() && AbilityToGet) {
			GA_Component->GiveAbility(FGameplayAbilitySpec(AbilityToGet, 0, InputID));
		}
		GA_Component->InitAbilityActorInfo(this, this);
	}
}
void AAI_01::HealthChanged(const FOnAttributeChangeData& Data)
{
		AAI_Controller* controller = Cast<AAI_Controller>(GetController());
		if (GetHelth() != 0.0f && GetHelth() > 0.0f) {
			UpdateWidget(GetHelth(), GetMaxHelth());
			if (controller) {
				//set Attack bool variable
				controller->setIsAttacking(true);
				//set variable for AIPerception
				controller->setIsHearSee(false);
				//Patrolling boll variable
				controller->setIsPatrol(false);
				//Write varibles in blackboard
				controller->getBlackBoard()->SetValueAsBool(TEXT("is_Patrol"), controller->getIsPatrol());
				controller->getBlackBoard()->SetValueAsBool(TEXT("is_Hear"), controller->getIsHearSee());
				controller->getBlackBoard()->SetValueAsBool(TEXT("is_Attacking"), controller->getIsAttacking());
				//controller->getBlackBoard()->SetValueAsObject(TEXT("Player"), DamageCauser);
			}
		}
		else {
			//AMyCharacter* player = Cast<AMyCharacter>(DamageCauser);
			this->Destroy();
			//Change Enemies amout
			//player->decreasenemycount();
		}
		//Call Bluprint Event after Recieve Damage
		DamageTaken();
}
//Set Movement Component Speed
void AAI_01::SetMaxWalkSpeed(float speed)
{
	this->GetCharacterMovement()->MaxWalkSpeed = speed;
}


