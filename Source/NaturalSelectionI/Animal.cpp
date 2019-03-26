// Fill out your copyright notice in the Description page of Project Settings.

#include "Animal.h"
#include "Engine/GameEngine.h"
#include "Components/InputComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AAnimal::AAnimal()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AAnimal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAnimal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAnimal::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// ==================================================== Set Functions are defined below ====================================================

void AAnimal::SetHealth(float health)
{
	this->Health = health;
}

void AAnimal::SetSpeed(float speed)
{
	this->Speed = speed;
}

void AAnimal::SetSize(float size)
{
	this->Size = size;
}

void AAnimal::SetDmg(float dmg)
{
	this->Dmg = dmg;
}

void AAnimal::SetDef(float def)
{
	this->Def = def;
}

void AAnimal::SetVision(float vision)
{
	this->Vision = vision;
}

float AAnimal::GetHealth()
{
	return this->Health;
}

float AAnimal::GetSpeed()
{
	return this->Speed;
}

float AAnimal::GetSize()
{
	return this->Size;
}

float AAnimal::GetDmg()
{
	return this->Dmg;
}

float AAnimal::GetDef()
{
	return this->Def;
}

float AAnimal::GetVision()
{
	return this->Vision;
}



