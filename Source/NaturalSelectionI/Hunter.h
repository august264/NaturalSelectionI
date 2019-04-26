// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "Gatherer.h"
#include "StateMachine.h"
#include "Hunter.generated.h"

/**
 * 
 */
UCLASS()
class NATURALSELECTIONI_API AHunter : public AAnimal
{
	GENERATED_BODY()

public:

	AHunter();


public:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* myMesh;

	// Called every frame
	virtual void Tick(float DeltaTime) override;



	FVector targetLocation;
	FVector position;

	void SetRandomTarget();
	void Wandering(float speed, float DeltaTime);
	void SpawnChild();
	void Chase();
	
	
	
	
};
