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

	// Set the states for FSM
	enum Hunter_States {
		DO_NOTHING_STATE = 0,
		WANDER_STATE,
		CHASE_STATE,
		ATTACK_STATE,
		EAT_STATE

	};

	AHunter();
private:
	StateMachine<Hunter_States, AHunter>* m_StateMachine;

	void State_Wander_OnEnter(void);
	void State_Wander_OnTick(float f_DeltaTime);
	void State_Wander_OnExit(void);

	void State_Chase_OnEnter(void);
	void State_Chase_OnTick(float f_DeltaTime);
	void State_Chase_OnExit(void);

	void State_Attack_OnEnter(void);
	void State_Attack_OnTick(float f_DeltaTime);
	void State_Attack_OnExit(void);

	void State_Eat_OnEnter(void);
	void State_Eat_OnTick(float f_DeltaTime);
	void State_Eat_OnExit(void);


public:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* myMesh;

	// Called every frame
	virtual void Tick(float DeltaTime) override;



	FVector targetLocation;
	FVector position;
	int Kills;

	void SetRandomTarget();
	void Wandering(float speed, float DeltaTime);
	void SpawnChild();
	void Chase();
	void DealDMG(AAnimal* Animal);
	 
	Hunter_States GetCurrState() { return m_StateMachine->GetCurrentState(); }
	void SetCurrState(Hunter_States state) { m_StateMachine->ChangeState(state); }
	
	
	
	
};
