// Fill out your copyright notice in the Description page of Project Settings.

#include "Food.h"
#include "ConstructorHelpers.h"
#include "Engine.h"


// Sets default values
AFood::AFood()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickupRoot"));
	RootComponent = PickupRoot;

	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	FoodMesh->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
	

	PickupBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupBox"));
	PickupBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	PickupBox->bGenerateOverlapEvents = true;
	PickupBox->OnComponentBeginOverlap.AddDynamic(this, &AFood::OnPlayerEnterPickupBox);
	PickupBox->AttachToComponent(PickupRoot, FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFood::OnPlayerEnterPickupBox(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	// OtherActor is the obj that is overlapping the current one
	// If a Gatherer is overlapping a food
	if (OtherActor->IsA(AGatherer::StaticClass())) {
		AGatherer* aOtherActor = Cast<AGatherer>(OtherActor);

		// Add 20 happiness to the Gatherer
		aOtherActor->SetHappiness(20);
		Destroy();
		

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("Moo See It, Moo Eat It, Moo Happy is")));
	}
	

	

}

