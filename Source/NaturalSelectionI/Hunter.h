// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
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

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	
	
	
};
