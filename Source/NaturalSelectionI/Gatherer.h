// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animal.h"
#include <vector>
#include "Food.h"
#include "StateMachine.h"
#include "Gatherer.generated.h"

/**
 * 
 */
UCLASS()
class NATURALSELECTIONI_API AGatherer : public AAnimal
{
	GENERATED_BODY()

public:

	// Set the states for FSM
	enum Gatherer_States {
		DO_NOTHING_STATE = 0,
		WANDER_STATE,
		EAT_STATE,
		ATTACK_STATE

	};

	AGatherer();
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	StateMachine<Gatherer_States, AGatherer>* m_StateMachine;

	void State_Wander_OnEnter(void);
	void State_Wander_OnTick(float f_DeltaTime);
	void State_Wander_OnExit(void);

	void State_Eat_OnEnter(void);
	void State_Eat_OnTick(float f_DeltaTime);
	void State_Eat_OnExit(void);

	void State_Attack_OnEnter(void);
	void State_Attack_OnTick(float f_DeltaTime);
	void State_Attack_OnExit(void);

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
	


	Gatherer_States GetCurrState() { return m_StateMachine->GetCurrentState(); }
	void SetCurrState(Gatherer_States state) { m_StateMachine->ChangeState(state); }

};
