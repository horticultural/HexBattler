// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexBattler/Public/Actors/Battler.h"
#include "HexGrid.generated.h"

class AHexTile;

USTRUCT(BlueprintType)
struct FHexGridY
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	TArray<AHexTile*> Tiles;
	
};



UCLASS()
class HEXBATTLER_API AHexGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexGrid();

protected:
	
	void SpawnUnits(int UnitQuantity, ETeam Team);

	void StartBattle();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FIntVector2 SimplePathFind(FIntVector2 Origin, FIntVector2 Target);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CreateHexGrid(int Width, int Height);

	UFUNCTION()
	void TickBattle();


public:
	
	/* The HexTile Actor that should be spawned for the grid. Must inherit from AHexTile*/
	UPROPERTY(EditAnywhere, Category="HexGridDetails")
    TSubclassOf<AHexTile> HexTileActor = nullptr;

	/* The width of the grid in # of tiles */
	UPROPERTY(EditAnywhere, Category="HexGridDetails")
	int HexGridWidth = 101;

	/* The height of the grid in # of tiles */
	UPROPERTY(EditAnywhere, Category="HexGridDetails")
	int HexGridHeight = 101;

	/* The radius of a single tiles in the grid visualiser. Needs to match with the FBX tile radius.*/
	UPROPERTY(EditAnywhere, Category="HexGridDetails")
	float HexagonRadius = 50.0;

	/* Seed # decides outcome of the battle based on pseudo random calculation */
	UPROPERTY(EditAnywhere, Category="HexGridDetails")
	int Seed = 1000;

	/* How many turns / timesteps have passed so far*/
	UPROPERTY(VisibleAnywhere, Category="HexGridDetails")
	int TurnNumber = 0;
	
	UPROPERTY(VisibleAnywhere, Category="HexGrid")
	TArray<FHexGridY> HexGrid2DArray;

	/* How many Red battlers should spawn */
	UPROPERTY(EditAnywhere, Category="BattleUnits")
	int RedTeamUnitsQuantity = 1;

	/* How many Blue battlers should spawn */
	UPROPERTY(EditAnywhere, Category="BattleUnits")
	int BlueTeamUnitsQuantity = 1;

	/* The Battler Class Actor that should be used for the units. Must inherit from ABattler*/
	UPROPERTY(EditAnywhere, Category="BattleUnits")
	TSubclassOf<ABattler> BattlerClass;

	/* The time step for each turn in seconds*/
	UPROPERTY(EditAnywhere, Category="BattleRules")
	float BattleTickTimeStep = 1.0;

private:
	
	FTimerHandle BattleTimerHandle;

	UPROPERTY()
	TArray<ABattler*> BattlerArray;

};
