// Name: Jianquan Li
// ID: 25924710


#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Animal.generated.h"

UCLASS()
class NATURALSELECTIONI_API AAnimal : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAnimal();

private:
	float Health, Speed, Size, Dmg, Def, Vision;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	void SetHealth(float health);
	void SetSpeed(float speed);
	void SetSize(float size);
	void SetDmg(float dmg);
	void SetDef(float def);
	void SetVision(float vision);

	// Acceessors below
	float GetHealth(); 
	float GetSpeed();
	float GetSize();
	float GetDmg();
	float GetDef();
	float GetVision();
};
