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

void ABattler::InitTeam_Implementation(ETeam Team)
{
	TeamName = Team;
}


// Called when the game starts or when spawned
void ABattler::BeginPlay()
{
	Super::BeginPlay();
}

void ABattler::Attack(int Seed, int TurnNumber, int BattlerNumber)
{
	int DamageToDo = ((Seed * TurnNumber * BattlerNumber) % 4 +2); //Pseudo random deterministic equation, based on the turn number and seed, looks random enough when I printed out the numbers!
	//UE_LOG(LogTemp,Warning, TEXT("Damage done: %d"), DamageToDo);
	
	NearestEnemyBattler->UpdateHealth(DamageToDo*-1); // *-1 because it's damage.
}

//Update heath rather than do damage so we can use the same equation for healing if need be in the future.
void ABattler::UpdateHealth(int DeltaHealth)
{
	Health = Health + DeltaHealth;
	OnDamaged.Broadcast();
	if(Health<=0)
	{
		Die();
	}
}

void ABattler::Die()
{
	Alive = false;
	OnDied.Broadcast();
}

// Called every frame
void ABattler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

