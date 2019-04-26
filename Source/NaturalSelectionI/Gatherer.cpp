// Fill out your copyright notice in the Description page of Project Settings.

#include "Gatherer.h"
#include <vector>
#include "Engine/GameEngine.h"
#include "Components/InputComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"


AGatherer::AGatherer() {

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// set the frame for this pawn
	// attach the myMesh to the root
	// and set the physics
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("myMesh"));
	myMesh->SetSimulatePhysics(true);
	myMesh->SetupAttachment(RootComponent);


	// then I created the finder for the mesh I want
	// set the mesh of myMesh to the one I want
	static ConstructorHelpers::FObjectFinder<UStaticMesh> myStaticMesh(TEXT("/Game/MyMaterials/Cow_ish"));

	myMesh = Cast<UStaticMeshComponent>(RootComponent);
	myMesh->SetStaticMesh(myStaticMesh.Object);

	// Set the basic property for Gatherer
	SetDmg(20.0f);
	SetSpeed(400.0f);
	SetHealth(100.0f);
	SetRandomTarget();


	// StateMachine setup and register
	m_StateMachine = new StateMachine<Gatherer_States, AGatherer>(this, DO_NOTHING_STATE);

	m_StateMachine->RegisterState(WANDER_STATE, &AGatherer::State_Wander_OnEnter, &AGatherer::State_Wander_OnTick, &AGatherer::State_Wander_OnExit);
	m_StateMachine->RegisterState(EAT_STATE, &AGatherer::State_Eat_OnEnter, &AGatherer::State_Eat_OnTick, &AGatherer::State_Eat_OnExit);
	m_StateMachine->RegisterState(ATTACK_STATE, &AGatherer::State_Attack_OnEnter, &AGatherer::State_Attack_OnTick, &AGatherer::State_Attack_OnExit);

	m_StateMachine->ChangeState(WANDER_STATE);


}

void AGatherer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_StateMachine->Tick(DeltaTime);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("HP: %f"), GetHealth()));

	if (GetHappiness() >= 100) {
		SpawnChild();	
	}
	if (GetHealth() <= 0) {
		Destroy();	
	}

}


void AGatherer::SetHappiness(float happy)
{
	Happiness += happy;
}

void AGatherer::SetRandomTarget()
{
	targetLocation.X = FMath::FRandRange(0, 3800);
	targetLocation.Y = FMath::FRandRange(0, 3800);
	targetLocation.Z = FMath::FRandRange(0, 0);
}

void AGatherer::Wandering(float speed, float DeltaTime)
{
	position = this->GetActorLocation();

	SetActorLocation(FMath::VInterpConstantTo(position, targetLocation, DeltaTime, GetSpeed()));
	if (FVector::Dist(position, targetLocation) < 100.0f)
	{
		SetRandomTarget();
	}
}

// Spawn an actor
void AGatherer::SpawnChild()
{
	FVector BirthLocation = GetActorLocation() + FVector(-150.0, -60.0f, 200.0f);

	AGatherer* Child = GetWorld()->SpawnActor<AGatherer>(GetClass(), BirthLocation, FRotator::ZeroRotator);

	// clear the happiness
	this->SetHappiness(-100.0f);
}

float AGatherer::GetHappiness()
{
	return Happiness;
}

void AGatherer::CauseDMG(AAnimal* animal)
{
	if (AHunter* hunter = Cast<AHunter>(animal)) {
		float myDmg = this->GetDmg();
		float newHealth = hunter->GetHealth() - myDmg;
		hunter->SetHealth(newHealth);
	}
}








// FSM functions are implemented below
void AGatherer::State_Wander_OnEnter(void)
{
}

void AGatherer::State_Wander_OnTick(float f_DeltaTime)
{
	Wandering(GetSpeed(), f_DeltaTime);

	int HunterNum;
	FVector HunterLocation;
	bool changeFlag = false;
	TArray<FHitResult> OutHits;
	FVector Location = GetActorLocation();

	// Gatherer sight is 300.0f/ 30m
	FCollisionShape Sphere = FCollisionShape::MakeSphere(300.0f);

	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, Sphere);
	if (isHit) {
		for (auto& Hit : OutHits) {
			// when any hit is found to be a AFood, set the flag to true and break, if no food is found, flag will remain false.
			if (AFood* food = Cast<AFood>(Hit.GetActor())) {
				changeFlag = true;
				break;			
			}			
		}

		std::vector<AHunter*> HunterList;
		for (auto& Hit : OutHits) {
			if (AHunter* Hunter = Cast<AHunter>(Hit.GetActor())) {
				HunterList.push_back(Hunter);
			}		
		}
		HunterNum = HunterList.size();
		if (HunterNum > 0) {
			HunterLocation = HunterList[0]->GetActorLocation();		
			if (FVector::Dist(this->GetActorLocation(), HunterLocation) < 100.0f) {
				m_StateMachine->ChangeState(ATTACK_STATE);
			}
			else
			{
				m_StateMachine->ChangeState(WANDER_STATE);
			}

		}
	}
	// if there is/are food nearby, change the state.
	if (changeFlag == true) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Going to eat")));
		m_StateMachine->ChangeState(EAT_STATE);
	}	
}

void AGatherer::State_Wander_OnExit(void)
{
}

void AGatherer::State_Eat_OnEnter(void)
{
}

void AGatherer::State_Eat_OnTick(float f_DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Eating")));

	// While in the Eat State, it needs to eat one of the food not all of them.
	// The food found nearby will be stored in the foodList, but only one will be used.
	
	int foodNum;
	
	FVector foodLocation;
	TArray<FHitResult> OutHits;
	FVector Location = GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(300.0f);
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, Sphere);
	if (isHit) {
		std::vector<AFood*> foodList;
		for (auto& Hit : OutHits) {
				if (AFood* food = Cast<AFood>(Hit.GetActor())) {
					foodList.push_back(food);
				}			
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("I see %d food"), foodList.size()));
		foodNum = foodList.size();
		if (foodNum > 0) {
			foodLocation = foodList[0]->GetActorLocation();
			SetActorLocation(FMath::VInterpConstantTo(this->GetActorLocation(), foodLocation, f_DeltaTime, GetSpeed()));
		}
		else {
			// Sadly there is no food around
			m_StateMachine->ChangeState(WANDER_STATE);
		
		}

	}
	
}

void AGatherer::State_Eat_OnExit(void)
{
}

void AGatherer::State_Attack_OnEnter(void)
{
}

void AGatherer::State_Attack_OnTick(float f_DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Attacking")));

	TArray<FHitResult> OutHits;
	FVector Location = GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(500.0f);
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, Sphere);

	if (isHit) {
		std::vector<AHunter*> HunterList;
		for (auto& Hit : OutHits) {
			if (AHunter* Hunter = Cast<AHunter>(Hit.GetActor())) {
				HunterList.push_back(Hunter);
			}		
		}
		if (HunterList.size() > 0) {
			if (HunterList[0]->GetHealth() > 0) {
				CauseDMG(HunterList[0]);			
			}
		
		}
	}
}

void AGatherer::State_Attack_OnExit(void)
{
}

