// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "Gatherer.generated.h"

/**
 * 
 */
UCLASS()
class NATURALSELECTIONI_API AGatherer : public AAnimal
{
	GENERATED_BODY()

public:
	AGatherer();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	float Happiness = 0.0f;


public:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* myMesh;

	void SetHappiness(float happy);
	

	FVector targetLocation;
	FVector position;
	void SetRandomTarget();
	void Wandering(float speed, float DeltaTime);
	void SpawnChild();
	float GetHappiness();
};
