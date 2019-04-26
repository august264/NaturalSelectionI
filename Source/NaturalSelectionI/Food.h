// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gatherer.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"
#include "Food.generated.h"

UCLASS()
class NATURALSELECTIONI_API AFood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	float checkFood();


	UPROPERTY(EditAnywhere)
		USceneComponent* PickupRoot;

	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* FoodMesh;

	UPROPERTY(EditAnywhere)
		UShapeComponent* PickupBox;

	UFUNCTION()
		void OnPlayerEnterPickupBox(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	
};
