// Fill out your copyright notice in the Description page of Project Settings.

#include "HexBattler/Public/Actors/HexGrid.h"

#include "NetworkMessage.h"
#include "HexBattler/Public/Actors/Battler.h"
#include "HexBattler/Public/Actors/HexTile.h"
#include "Perception/AISense_Team.h"



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
	/*TArray<AHexTile*> HexTilePath;

	GetNeighbourTiles(OriginX, OriginY);*/

	int XDistance = Target.X - Origin.X;
	int YDistance = Target.Y - Origin.Y;

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

// this is for A* path finding.
void AHexGrid::GetNeighbourTiles(int TileX, int TileY)
{
	TArray<AHexTile*> NeighbourTiles;
	
	
	NeighbourTiles.Add(HexGrid2DArray[TileX].Tiles[TileY-1]); // TODO make sure these fall within scope.
	NeighbourTiles.Add(HexGrid2DArray[TileX].Tiles[TileY+1]);
	NeighbourTiles.Add(HexGrid2DArray[TileX-1].Tiles[TileY]);
	NeighbourTiles.Add(HexGrid2DArray[TileX+1].Tiles[TileY]);
	NeighbourTiles.Add(HexGrid2DArray[TileX-1].Tiles[TileY+1]);
	NeighbourTiles.Add(HexGrid2DArray[TileX-1].Tiles[TileY+1]);

	for (AHexTile* HexTile : NeighbourTiles)
	{
		//if()
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
		NewBattler->Team = Team; //TODO: change this to have a spawn argument. so that we can init team colours etc in ABattler
		NewBattler->TileLocation = FIntVector2(NewBattlerStartTileX,NewBattlerStartTileY);


		BattlerArray.Add(NewBattler);
		//TODO: I don't really like these if statements here. Would be better to have and Array of Battler Arrays so I could iterate through as many different teams as is required.
		if(Team == ETeam::Red) //TODO: I might just get rid of this.
		{
			RedBattlerArray.Add(NewBattler);
		}
		else if(Team == ETeam::Blue)
		{
			BlueBattlerArray.Add(NewBattler);
		}
	}
}

void AHexGrid::StartBattle()
{
	GetWorld()->GetTimerManager().SetTimer(BattleTimerHandle, this, &AHexGrid::TickBattle, BattleTickTimeStep, true);
}

void AHexGrid::TickBattle()
{
	//UE_LOG(LogTemp, Warning, TEXT("Turn # %d"), TurnNumber);
	TurnNumber++;
	for (ABattler* Battler : BattlerArray)
	{
		if(Battler->Alive && Battler->ActOnTurnNumber == 0)
		{
			int ClosestEnemyDistance = 100;
			for(ABattler* EnemyBattler : BattlerArray)
			{
				if(Battler != EnemyBattler && Battler->Team != EnemyBattler->Team && EnemyBattler->Alive==true) // check the other battler isn't on the same team.
				{
					Battler->TileLocation;
					EnemyBattler->TileLocation;
					int TilesBetweenBattlers = FMath::Abs((Battler->TileLocation.X - EnemyBattler->TileLocation.X))  + FMath::Abs((Battler->TileLocation.Y - EnemyBattler->TileLocation.Y));
					if(TilesBetweenBattlers < ClosestEnemyDistance)
					{
						ClosestEnemyDistance = TilesBetweenBattlers;
						Battler->NearestEnemyBattler = EnemyBattler;
					}
				}
			}

			Battler->TurnMode = (ClosestEnemyDistance <= Battler->TileRange) ? ETurnMode::Attack :  ETurnMode::MoveToNextTile;
			Battler->ActOnTurnNumber = TurnNumber + ((ClosestEnemyDistance <= Battler->TileRange) ? Battler->TurnsPerAttack : Battler->TurnsPerMove);
		}
		/*if(ClosestEnemyDistance <= Battler->TileRange)
		{
			Battler->TurnMode = ETurnMode::Attack;
		}
		else
		{
			Battler->TurnMode = ETurnMode::Move;

		}*/
	}

	for (ABattler* Battler : BattlerArray)
	{
		if(Battler->Alive && Battler->TurnMode == ETurnMode::Attack && Battler->ActOnTurnNumber == TurnNumber) // Because of the order of battlers in the Array this favours one team essentially attacking first.
		{
			int BattlerNumber = BattlerArray.Find(Battler);
			Battler->Attack(Seed, TurnNumber, BattlerNumber);
			Battler->ActOnTurnNumber = 0;
		}
	}

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
	
	/*for (ABattler* RedBattler : RedBattlerArray)
	{
		int ClosestEnemyDistance = 100;
		for(ABattler* EnemyBattler : BlueBattlerArray)
		{
			RedBattler->TileLocation;
			EnemyBattler->TileLocation;
			int TilesBetweenBattlers = FMath::Abs((RedBattler->TileLocation.X - EnemyBattler->TileLocation.X))  + FMath::Abs((RedBattler->TileLocation.Y - EnemyBattler->TileLocation.Y));
			if(TilesBetweenBattlers < ClosestEnemyDistance)
			{
				ClosestEnemyDistance = TilesBetweenBattlers;
				RedBattler->NearestEnemyBattler = EnemyBattler;
			}
		}
	}

	for (ABattler* BlueBattler : BlueBattlerArray)
	{
		int ClosestEnemyDistance = 100;
		for(ABattler* EnemyBattler : RedBattlerArray)
		{
			BlueBattler->TileLocation;
			EnemyBattler->TileLocation;
			int TilesBetweenBattlers = FMath::Abs((BlueBattler->TileLocation.X - EnemyBattler->TileLocation.X))  + FMath::Abs((BlueBattler->TileLocation.Y - EnemyBattler->TileLocation.Y));
			if(TilesBetweenBattlers < ClosestEnemyDistance) // TODO: Account for if there are more than one which are the same distance away and choose one? Although choosing the first one in the list helps the Deterministic-ness
			{
				ClosestEnemyDistance = TilesBetweenBattlers;
				BlueBattler->NearestEnemyBattler = EnemyBattler;
			}
		}
	}*/
	// For each living Battler
	// find closest enemy Battler
	// If in range Battler->Shoot()
	// Else path to enemy Battler and move one step in that direction.
	
}