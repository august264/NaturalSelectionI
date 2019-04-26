// Fill out your copyright notice in the Description page of Project Settings.

#include "Hider.h"
#include "Engine/GameEngine.h"
#include "Components/InputComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"

AHider::AHider() {
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("myMesh"));
	myMesh->SetSimulatePhysics(true);
	myMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> myStaticMesh(TEXT("/Game/StarterContent/Shapes/Shape_Torus.Shape_Torus"));

	myMesh = Cast<UStaticMeshComponent>(RootComponent);
	myMesh->SetStaticMesh(myStaticMesh.Object);


	// weak but strong defence
	SetDmg(10.0f);
	SetSpeed(800.0f);
	SetHealth(500.0f);
	SetRandomTarget();

	m_StateMachine = new StateMachine<Hider_States, AHider>(this,DO_NOTHING_STATE);

	m_StateMachine->RegisterState(WANDER_STATE, &AHider::State_Wander_OnEnter, &AHider::State_Wander_OnTick, &AHider::State_Wander_OnExit);
	m_StateMachine->RegisterState(FLEE_STATE, &AHider::State_Flee_OnEnter, &AHider::State_Flee_OnTick, &AHider::State_Flee_OnExit);
	m_StateMachine->RegisterState(ATTACK_STATE, &AHider::State_Attack_OnEnter, &AHider::State_Attack_OnTick, &AHider::State_Attack_OnExit);

	m_StateMachine->ChangeState(WANDER_STATE);

	spawntime = 0.0f;


}

void AHider::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);
	m_StateMachine->Tick(DeltaTime);

	if (this->GetHealth() <= 0) {
		Destroy();
	}
	
	spawntime += DeltaTime;
	if (spawntime >= 60) {
		SpawnChild();
		spawntime = 0;
	}
}



void AHider::State_Wander_OnEnter(void)
{
}

void AHider::State_Wander_OnTick(float f_DeltaTime)
{
	Wandering(GetSpeed(), f_DeltaTime);

	bool changeFlag = false;

	TArray<FHitResult> OutHits;
	FVector Location = GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(300.0f);

	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, Sphere);

	if (isHit) {
		for (auto& Hit : OutHits) {
			if (((Hit.GetActor()->IsA(AGatherer::StaticClass())) || (Hit.GetActor()->IsA(AHunter::StaticClass())) || (Hit.GetActor()->IsA(AHider::StaticClass()))) && Hit.GetActor() != this) {
				changeFlag = true;
				break;				
			}
		}		
	}

	if (changeFlag == true) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Going to eat")));
		m_StateMachine->ChangeState(FLEE_STATE);
	}

}

void AHider::State_Wander_OnExit(void)
{
}

void AHider::State_Flee_OnEnter(void)
{
}

void AHider::State_Flee_OnTick(float f_DeltaTime)
{

	int HunterNum;
	FVector HunterLocation;

	TArray<FHitResult> OutHits;
	FVector Location = this->GetActorLocation();
	FCollisionShape Sphere = FCollisionShape::MakeSphere(500.0f);
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, Location, Location, FQuat::Identity, ECC_WorldStatic, Sphere);
	if (isHit) {
		std::vector<AActor*> AvoidList;
		for (auto& Hit : OutHits) {
			if (((Hit.GetActor()->IsA(AGatherer::StaticClass())) || (Hit.GetActor()->IsA(AHunter::StaticClass())) || (Hit.GetActor()->IsA(AHider::StaticClass()))) && Hit.GetActor() != this) {
				AvoidList.push_back(Hit.GetActor());
			}
		}
		HunterNum = AvoidList.size();

		if (HunterNum > 0) {
			HunterLocation = AvoidList[0]->GetActorLocation();
			FVector enemyLoc = HunterLocation;
			HunterLocation.X *= -1.5;
			HunterLocation.Y *= -1.5;
			SetActorLocation(FMath::VInterpConstantTo(this->GetActorLocation(), HunterLocation, f_DeltaTime, GetSpeed()));
			if (FVector::Dist(this->GetActorLocation(), enemyLoc) > 150.0f) {
				m_StateMachine->ChangeState(WANDER_STATE);
			}

			if (FVector::Dist(this->GetActorLocation(), enemyLoc) < 150.0f)
			{
				m_StateMachine->ChangeState(ATTACK_STATE);
			}
			
		}
	}


}

void AHider::State_Flee_OnExit(void)
{
}

void AHider::State_Attack_OnEnter(void)
{
}

void AHider::State_Attack_OnTick(float f_DeltaTime)
{
	
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
				time_elapsed += f_DeltaTime;
				if (time_elapsed >= 1.0f) {
					DealDMG(HunterList[0]);
					time_elapsed = 0;
				}
			}

		}
		else
		{
			m_StateMachine->ChangeState(WANDER_STATE);
		}
	}
}

void AHider::State_Attack_OnExit(void)
{
}

void AHider::SetRandomTarget()
{
	targetLocation.X = FMath::FRandRange(0, 3800);
	targetLocation.Y = FMath::FRandRange(0, 3800);
	targetLocation.Z = FMath::FRandRange(0, 0);
}

void AHider::Wandering(float speed, float DeltaTime)
{
	position = this->GetActorLocation();

	SetActorLocation(FMath::VInterpConstantTo(position, targetLocation, DeltaTime, GetSpeed()));
	if (FVector::Dist(position, targetLocation) < 100.0f)
	{
		SetRandomTarget();
	}
}

void AHider::SpawnChild()
{
	FVector BirthLocation = GetActorLocation() + FVector(-150.0, -60.0f, 200.0f);

	AHider* Child = GetWorld()->SpawnActor<AHider>(GetClass(), BirthLocation, FRotator::ZeroRotator);
}

void AHider::Flee()
{
}

void AHider::DealDMG(AAnimal* Animal)
{

	if (AHunter* hunter = Cast<AHunter>(Animal)) {
		float myDmg = this->GetDmg();
		float newHealth = hunter->GetHealth() - myDmg;
		hunter->SetHealth(newHealth);
	}
	
}
