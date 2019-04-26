// Fill out your copyright notice in the Description page of Project Settings.

#include "Hunter.h"
#include "Engine/GameEngine.h"
#include <vector>
#include <ctime>
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/InputComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"




AHunter::AHunter() {

	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("myMesh"));
	myMesh->SetSimulatePhysics(true);
	myMesh->SetupAttachment(RootComponent);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> myStaticMesh(TEXT("/Game/MyMaterials/Hunter_ish.Hunter_ish"));

	myMesh = Cast<UStaticMeshComponent>(RootComponent);
	myMesh->SetStaticMesh(myStaticMesh.Object);

	Kills = 0;
	SetDmg(25.0f);
	SetSpeed(700.0f);
	SetHealth(100.0f);
	SetRandomTarget();

	m_StateMachine = new StateMachine<Hunter_States, AHunter>(this, DO_NOTHING_STATE);

	m_StateMachine->RegisterState(WANDER_STATE, &AHunter::State_Wander_OnEnter, &AHunter::State_Wander_OnTick, &AHunter::State_Wander_OnExit);
	m_StateMachine->RegisterState(CHASE_STATE, &AHunter::State_Chase_OnEnter, &AHunter::State_Chase_OnTick, &AHunter::State_Chase_OnExit);
	m_StateMachine->RegisterState(ATTACK_STATE, &AHunter::State_Attack_OnEnter, &AHunter::State_Attack_OnTick, &AHunter::State_Attack_OnExit);
	m_StateMachine->RegisterState(EAT_STATE, &AHunter::State_Eat_OnEnter, &AHunter::State_Eat_OnTick, &AHunter::State_Eat_OnExit);

	m_StateMachine->ChangeState(WANDER_STATE);
}

void AHunter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_StateMachine->Tick(DeltaTime);

	if (this->Kills >= 2) {
		SpawnChild();
	}

}

void AHunter::State_Wander_OnEnter(void)
{
}

void AHunter::State_Wander_OnTick(float f_DeltaTime)
{
	Wandering(GetSpeed(), f_DeltaTime);

	bool changeFlag = false;
	
	TArray<FHitResult> OutHits;
	FVector Location = GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(300.0f);

	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, Sphere);

	if (isHit) {
		for (auto& Hit : OutHits) {
			// when any hit is found to be a AGatherer, set the flag to true and break, if no food is found, flag will remain false.
			if (AGatherer* Gatherer = Cast<AGatherer>(Hit.GetActor())) {
				changeFlag = true;
				break;			
			}
		}	
	}

	if (changeFlag == true) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Going to eat")));
		m_StateMachine->ChangeState(CHASE_STATE);
	}
}

void AHunter::State_Wander_OnExit(void)
{
}

void AHunter::State_Chase_OnEnter(void)
{
}

void AHunter::State_Chase_OnTick(float f_DeltaTime)
{
	int preyNum;
	FVector PreyLocation;
	TArray<FHitResult> OutHits;
	FVector Location = GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(500.0f);
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, Sphere);
	if (isHit) {
		std::vector<AAnimal*> PreyList;
		for (auto& Hit : OutHits) {
			if (AGatherer* Gatherer = Cast<AGatherer>(Hit.GetActor())) {
				PreyList.push_back(Gatherer);
			}		
		}
		preyNum = PreyList.size();
		if (preyNum > 0) {
			PreyLocation = PreyList[0]->GetActorLocation();
			SetActorLocation(FMath::VInterpConstantTo(this->GetActorLocation(), PreyLocation, f_DeltaTime, GetSpeed()));
			if (FVector::Dist(this->GetActorLocation(), PreyLocation) < 100.0f) {
				m_StateMachine->ChangeState(ATTACK_STATE);
			}
			else
			{
				m_StateMachine->ChangeState(WANDER_STATE);
			}
		}
	}
		
}

void AHunter::State_Chase_OnExit(void)
{
}

void AHunter::State_Attack_OnEnter(void)
{
}

void AHunter::State_Attack_OnTick(float f_DeltaTime)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Attacking")));


	int preyNum;
	TArray<FHitResult> OutHits;
	FVector Location = GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(500.0f);
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, Sphere);

	if (isHit) {
		std::vector<AAnimal*> PreyList;
		for (auto& Hit : OutHits) {
			if (AGatherer* Gatherer = Cast<AGatherer>(Hit.GetActor())) {
				PreyList.push_back(Gatherer);
			}		
		}
		preyNum = PreyList.size();
		if (preyNum > 0) {
			if (PreyList[0]->GetHealth() > 0) {	
				this->DealDMG(PreyList[0]);
				if (PreyList[0]->GetHealth() <= 0) {
					this->Kills += 1;
					m_StateMachine->ChangeState(EAT_STATE);
				}
			}	
		}
	}
}

void AHunter::State_Attack_OnExit(void)
{
}

void AHunter::State_Eat_OnEnter(void)
{
}

void AHunter::State_Eat_OnTick(float f_DeltaTime)
{
	
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Eating Time!!!!!")));
	m_StateMachine->ChangeState(WANDER_STATE);
}

void AHunter::State_Eat_OnExit(void)
{
}



void AHunter::SetRandomTarget()
{
	targetLocation.X = FMath::FRandRange(0, 3800);
	targetLocation.Y = FMath::FRandRange(0, 3800);
	targetLocation.Z = FMath::FRandRange(0, 0);
}

void AHunter::Wandering(float speed, float DeltaTime)
{
	position = this->GetActorLocation();

	SetActorLocation(FMath::VInterpConstantTo(position, targetLocation, DeltaTime, GetSpeed()));
	if (FVector::Dist(position, targetLocation) < 100.0f)
	{
		SetRandomTarget();
	}
}

void AHunter::SpawnChild()
{
	FVector BirthLocation = GetActorLocation() + FVector(-150.0, -60.0f, 200.0f);

	AHunter* Child = GetWorld()->SpawnActor<AHunter>(GetClass(), BirthLocation, FRotator::ZeroRotator);
	this->Kills = 0;
	
}

void AHunter::Chase()
{
}

void AHunter::DealDMG(AAnimal* Animal)
{
	float myDmg = this->GetDmg();
	float newHealth = Animal->GetHealth() - myDmg;
	Animal->SetHealth(newHealth);
}

