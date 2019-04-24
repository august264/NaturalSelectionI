// Fill out your copyright notice in the Description page of Project Settings.

#include "ZooKeeper.h"


// Sets default values
AZooKeeper::AZooKeeper()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AZooKeeper::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AZooKeeper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZooKeeper::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

