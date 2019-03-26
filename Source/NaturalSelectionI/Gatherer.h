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
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* myMesh;
	
	
};
