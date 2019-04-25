// Fill out your copyright notice in the Description page of Project Settings.

#include "Gatherer.h"
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
	
	SetSpeed(400.0f);
	SetHealth(100.0f);
	SetRandomTarget();


	// StateMachine setup and register
	m_StateMachine = new StateMachine<Gatherer_States, AGatherer>(this, WANDER_STATE);

	m_StateMachine->RegisterState(WANDER_STATE, &AGatherer::State_Wander_OnEnter, &AGatherer::State_Wander_OnTick, &AGatherer::State_Wander_OnExit);
	m_StateMachine->RegisterState(EAT_STATE, &AGatherer::State_Eat_OnEnter, &AGatherer::State_Eat_OnTick, &AGatherer::State_Eat_OnExit);
	m_StateMachine->RegisterState(ATTACK_STATE, &AGatherer::State_Attack_OnEnter, &AGatherer::State_Attack_OnTick, &AGatherer::State_Attack_OnExit);
	
	m_StateMachine->ChangeState(WANDER_STATE);
	
	
}

void AGatherer::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

	//Wandering(GetSpeed(), DeltaTime);
	//if (this->GetHappiness() >= 100.0f) {
	//	SpawnChild();
    //	
	//}

	m_StateMachine->Tick(DeltaTime);
	

}


void AGatherer::SetHappiness(float happy)
{
	Happiness += happy;
}

void AGatherer::SetRandomTarget()
{
	targetLocation.X = FMath::FRandRange(0, 3800);
	targetLocation.Y = FMath::FRandRange(0, 3800);
	targetLocation.Z = FMath::FRandRange(0,0);
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

void AGatherer::FindandEat(float speed, float DeltaTime)
{
	TArray<FHitResult> OutHits;
	FVector Location = GetActorLocation();

	// Gatherer sight is 300.0f/ 30m
	FCollisionShape Sphere = FCollisionShape::MakeSphere(300.0f);

	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, Sphere);
	if (isHit) {
		for (auto& Hit : OutHits) {
			AFood* food = Cast<AFood>(Hit.GetActor());
			if (food) {
				FVector toFood = food->GetActorLocation();
				SetActorLocation(FMath::VInterpConstantTo(GetActorLocation(), toFood, DeltaTime, GetSpeed()));
				break;
			}
		}
	}
}


// FSM functions are implemented below
void AGatherer::State_Wander_OnEnter(void)
{
}

void AGatherer::State_Wander_OnTick(float f_DeltaTime)
{
	Wandering(GetSpeed(),f_DeltaTime);

	TArray<FHitResult> OutHits;
	FVector Location = GetActorLocation();

	// Gatherer sight is 300.0f/ 30m
	FCollisionShape Sphere = FCollisionShape::MakeSphere(300.0f);

	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, Sphere);
	if (isHit) {
		

		for (auto& Hit : OutHits) {
			if (Hit.GetActor()->IsA(AFood::StaticClass())) {
				
				m_StateMachine->ChangeState(EAT_STATE);
				break;
			}
		}
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
	FindandEat(GetSpeed(), f_DeltaTime);
	m_StateMachine->ChangeState(WANDER_STATE);

}

void AGatherer::State_Eat_OnExit(void)
{
}

void AGatherer::State_Attack_OnEnter(void)
{
}

void AGatherer::State_Attack_OnTick(float f_DeltaTime)
{
}

void AGatherer::State_Attack_OnExit(void)
{
}



