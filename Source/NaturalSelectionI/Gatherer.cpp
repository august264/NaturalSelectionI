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

	// then i created the finder for the mesh i want
	// set the mesh of myMesh to the one I want
	static ConstructorHelpers::FObjectFinder<UStaticMesh> myStaticMesh(TEXT("/Game/MyMaterials/Cow_ish"));
	
	myMesh = Cast<UStaticMeshComponent>(RootComponent);
	myMesh->SetStaticMesh(myStaticMesh.Object);
	
	
}

