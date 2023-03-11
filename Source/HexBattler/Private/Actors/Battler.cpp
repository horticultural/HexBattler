// Fill out your copyright notice in the Description page of Project Settings.


#include "HexBattler/Public/Actors//Battler.h"

// Sets default values
ABattler::ABattler()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;	
}



// Called when the game starts or when spawned
void ABattler::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABattler::Attack(int Seed, int TurnNumber, int BattlerNumber)
{
	//Show some animation of a shot toward the enemy battler.

	int DamageToDo = ((Seed * TurnNumber * BattlerNumber) % 4 +2);
	//UE_LOG(LogTemp,Warning, TEXT("Damage done: %d"), DamageToDo);
	NearestEnemyBattler->UpdateHealth(DamageToDo*-1);
}

void ABattler::UpdateHealth(int DeltaHealth)
{
	Health = Health + DeltaHealth;
	if(Health<=0)
	{
		Die();
	}
}

void ABattler::Die_Implementation()
{
	Alive = false;
	//TODO: Change the colour to grey or something. Do it in BP.
}

// Called every frame
void ABattler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

