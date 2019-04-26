// Fill out your copyright notice in the Description page of Project Settings.

#include "Hunter.h"
#include "Engine/GameEngine.h"
#include "Components/InputComponent.h"
#include "ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"




AHunter::AHunter() {

	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RootComponent"));
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("myMesh"));
	myMesh->SetSimulatePhysics(true);
	myMesh->SetupAttachment(RootComponent);

	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> myStaticMesh(TEXT("/Game/StarterContent/Shapes/Shape_Trim_90_In.Shape_Trim_90_In"));

	myMesh = Cast<UStaticMeshComponent>(RootComponent);
	myMesh->SetStaticMesh(myStaticMesh.Object);

	SetSpeed(700.0f);
	SetHealth(100.0f);
	SetRandomTarget();
	

}

void AHunter::Tick(float DeltaTime)
{
	Wandering(GetSpeed(), DeltaTime);
	Super::Tick(DeltaTime);
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

