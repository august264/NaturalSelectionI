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
	//SetRandomTarget();
	
	
}

void AGatherer::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);

	Wandering(GetSpeed(), DeltaTime);
	if (this->GetHappiness() >= 100.0f) {
		//SpawnChild();
	
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
	targetLocation.Z = FMath::FRandRange(0,0);
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("My Happiness: %f"), this->GetHappiness()));

}

void AGatherer::Wandering(float speed, float DeltaTime)
{
	position = this->GetActorLocation();
	
	SetActorLocation(FMath::VInterpConstantTo(position, targetLocation, DeltaTime, GetSpeed()));
	if (FVector::Dist(position, targetLocation) < 200.0f)
	{
		SetRandomTarget();
				
	}
}

void AGatherer::SpawnChild()
{
	
	FVector BirthLocation = GetActorLocation() + FVector(-150.0, -60.0f, 200.0f);

	AGatherer* Child = GetWorld()->SpawnActor<AGatherer>(GetClass(), BirthLocation, FRotator::ZeroRotator);

	// clear the happiness
	this->SetHappiness(-100);

	
}

float AGatherer::GetHappiness()
{
	return Happiness;
}


