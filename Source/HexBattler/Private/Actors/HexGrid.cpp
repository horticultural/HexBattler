// Fill out your copyright notice in the Description page of Project Settings.

#include "HexBattler/Public/Actors/HexGrid.h"
#include "HexBattler/Public/Actors/Battler.h"
#include "HexBattler/Public/Actors/HexTile.h"



// Sets default values
AHexGrid::AHexGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHexGrid::BeginPlay()
{
	Super::BeginPlay();
	
	CreateHexGrid(HexGridWidth, HexGridHeight);


	//Spawn Units for each team.

	SpawnUnits(RedTeamUnitsQuantity, ETeam::Red);
	SpawnUnits(BlueTeamUnitsQuantity, ETeam::Blue);

	//Start Game Tick logic.

	StartBattle();
}



// Called every frame
void AHexGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHexGrid::CreateHexGrid(int Width, int Height)
{
	if(!HexTileActor){return;}

	HexGrid2DArray.Empty();

	//Loop to generate and spawn the hex grid system. TODO:change spawns to either a dynamic Shader or to instanced/procedural mesh so it's more optimised. having 1000 hexes spawn for a 100x100 grid is too much.
	for (int i = 0; i < Width; i++)
	{
		HexGrid2DArray.Add(FHexGridY());
		for (int j = 0; j < Height; j++)
		{
			AHexTile* HexTile = GetWorld()->SpawnActor<AHexTile>(HexTileActor);
			FVector Location = FVector::ZeroVector;
			HexGrid2DArray[i].Tiles.Add(HexTile);
			
			Location.X = sqrt(3) * HexagonRadius * i +(j%2 * sqrt(3) * HexagonRadius/2) ; // X and Y location coord calcs to account for the offsetting of the grid on each line.
			Location.Y = 0.75 * HexagonRadius * 2 * j;
			
			HexTile->SetActorLocation(Location);

			FText CoordText =FText::FromString(FString::Printf(TEXT("%d, %d"), i,j)); // debug
			HexTile->SetText(CoordText);
		}
	}
}

FIntVector2 AHexGrid::SimplePathFind(FIntVector2 Origin, FIntVector2 Target)
{
	int XDistance = Target.X - Origin.X;
	int YDistance = Target.Y - Origin.Y;

	//This is a really simple path finding system. Essentially just moves in the X direction first, then Y once it reaches the same X location.
	if (XDistance > 0)
	{
		return FIntVector2(Origin.X+1, Origin.Y);
	}
	else if(XDistance < 0)
	{
		return FIntVector2(Origin.X-1, Origin.Y);
	}
	else if (YDistance > 0)
	{
		return FIntVector2(Origin.X, Origin.Y+1);
	}
	else if(YDistance < 0)
	{
		return FIntVector2(Origin.X, Origin.Y-1);
	}
	else
	{
		return FIntVector2(Origin.X, Origin.Y); // Something's gone wrong if this happens.
	}
	
}

void AHexGrid::SpawnUnits(int UnitQuantity, ETeam Team)
{
	if(!BattlerClass){return;}
	
	for (int i = 0; i < UnitQuantity; i++)
	{
		ABattler* NewBattler = GetWorld()->SpawnActor<ABattler>(BattlerClass);

		//This is some nonsense equation to get a pseudorandom number deterministically. When playing the game it looks random enough, without any discernible pattern.
		int NewBattlerStartTileX = (Seed * (i+1)^4 * i+1 * 23 + 12 * 39 * Team*4) % HexGridWidth;//FMath::RandRange( 0,  HexGridWidth-1);
		int NewBattlerStartTileY = (Seed * (i+1)^4 * 12 + 63 * 23 * Team*4) % HexGridHeight;//FMath::RandRange(0, HexGridHeight-1);

		FVector NewBattlerSpawnLocation = HexGrid2DArray[NewBattlerStartTileX].Tiles[NewBattlerStartTileY]->GetActorLocation();
		NewBattler->SetActorLocation(NewBattlerSpawnLocation);
		NewBattler->InitTeam(Team);
		NewBattler->TileLocation = FIntVector2(NewBattlerStartTileX,NewBattlerStartTileY);


		BattlerArray.Add(NewBattler);
	}
}

void AHexGrid::StartBattle()
{
	GetWorld()->GetTimerManager().SetTimer(BattleTimerHandle, this, &AHexGrid::TickBattle, BattleTickTimeStep, true);
}

void AHexGrid::TickBattle()
{
	//UE_LOG(LogTemp, Warning, TEXT("Turn # %d"), TurnNumber);
	
	//Increase the turn number
	TurnNumber++;

	// Iterate through all Battlers, Assigning whether the will attack or move next. 
	for (ABattler* Battler : BattlerArray)
	{
		if(Battler->Alive && Battler->ActOnTurnNumber == 0)
		{
			int ClosestEnemyDistance = 1000; // an arbitrarily large number
			for(ABattler* EnemyBattler : BattlerArray)
			{
				if(Battler != EnemyBattler && Battler->TeamName != EnemyBattler->TeamName && EnemyBattler->Alive==true) // check the other battler isn't on the same team.
				{
					int TilesBetweenBattlers = FMath::Abs((Battler->TileLocation.X - EnemyBattler->TileLocation.X))  + FMath::Abs((Battler->TileLocation.Y - EnemyBattler->TileLocation.Y)); // Finds distance between battlers.
					if(TilesBetweenBattlers < ClosestEnemyDistance)
					{
						ClosestEnemyDistance = TilesBetweenBattlers;
						Battler->NearestEnemyBattler = EnemyBattler;
					}
				}
			}

			if(ClosestEnemyDistance <= Battler->TileRange) //Attack Next
			{
				Battler->TurnMode = ETurnMode::Attack;
				Battler->ActOnTurnNumber = TurnNumber + Battler->TurnsPerAttack;
				Battler->StartAction(Battler->NearestEnemyBattler->GetActorLocation(), BattleTickTimeStep * Battler->TurnsPerAttack);
			}
			else // Move Next
			{
				Battler->TurnMode = ETurnMode::MoveToNextTile;
				Battler->ActOnTurnNumber = TurnNumber + Battler->TurnsPerMove;
				FIntVector2 TileToMoveTo = SimplePathFind(Battler->TileLocation, Battler->NearestEnemyBattler->TileLocation);
				FVector TargetLocation = HexGrid2DArray[TileToMoveTo.X].Tiles[TileToMoveTo.Y]->GetActorLocation();
				Battler->StartAction(TargetLocation, BattleTickTimeStep * Battler->TurnsPerMove);

			}
		}
	}

	//Attack Loop
	for (ABattler* Battler : BattlerArray)
	{
		if(Battler->Alive && Battler->TurnMode == ETurnMode::Attack && Battler->ActOnTurnNumber == TurnNumber) // Because of the order of battlers in the Array this favours one team essentially attacking first.
		{
			int BattlerNumber = BattlerArray.Find(Battler);
			Battler->Attack(Seed, TurnNumber, BattlerNumber);
			Battler->ActOnTurnNumber = 0;
		}
	}

	//MoveLoop
	for (ABattler* Battler : BattlerArray)
	{
		if(Battler->Alive && Battler->TurnMode == ETurnMode::MoveToNextTile && Battler->ActOnTurnNumber == TurnNumber)
		{
			if(Battler->NearestEnemyBattler->Alive)
			{
				FIntVector2 TileToMoveTo = SimplePathFind(Battler->TileLocation, Battler->NearestEnemyBattler->TileLocation);
				Battler->TileLocation = TileToMoveTo;
				Battler->SetActorLocation(HexGrid2DArray[TileToMoveTo.X].Tiles[TileToMoveTo.Y]->GetActorLocation());
			}
			Battler->ActOnTurnNumber = 0;
		}
	}
}