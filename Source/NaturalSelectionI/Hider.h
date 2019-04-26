// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include "Gatherer.h"
#include "Hunter.h"
#include "StateMachine.h"
#include "Hider.generated.h"

/**
 * 
 */
UCLASS()
class NATURALSELECTIONI_API AHider : public AAnimal
{
	GENERATED_BODY()

public:
	enum Hider_States {
		DO_NOTHING_STATE = 0,
		WANDER_STATE,
		FLEE_STATE,
		ATTACK_STATE

	};

	AHider();

private:
	StateMachine<Hider_States, AHider>* m_StateMachine;

	void State_Wander_OnEnter(void);
	void State_Wander_OnTick(float f_DeltaTime);
	void State_Wander_OnExit(void);

	void State_Flee_OnEnter(void);
	void State_Flee_OnTick(float f_DeltaTime);
	void State_Flee_OnExit(void);

	void State_Attack_OnEnter(void);
	void State_Attack_OnTick(float f_DeltaTime);
	void State_Attack_OnExit(void);


public:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* myMesh;


	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float time_elapsed = 0.0f;
	float spawntime;
	FVector targetLocation;
	FVector position;

	void SetRandomTarget();
	void Wandering(float speed, float DeltaTime);
	void SpawnChild();
	void Flee();

	void DealDMG(AAnimal* Animal);

	Hider_States GetCurrState() { return m_StateMachine->GetCurrentState(); }
	void SetCurrState(Hider_States state) { m_StateMachine->ChangeState(state); }
	
	
};
