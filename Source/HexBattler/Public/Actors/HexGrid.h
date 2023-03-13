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
	

	UPROPERTY(EditAnywhere, Category="HexGridDetails")
    TSubclassOf<AHexTile> HexTileActor = nullptr;
    	
	UPROPERTY(EditAnywhere, Category="HexGridDetails")
	int HexGridWidth = 101;

	UPROPERTY(EditAnywhere, Category="HexGridDetails")
	int HexGridHeight = 101;

	UPROPERTY(EditAnywhere, Category="HexGridDetails")
	float HexagonRadius = 50.0;
	
	UPROPERTY(EditAnywhere, Category="HexGridDetails")
	int Seed = 1000;

	UPROPERTY(VisibleAnywhere, Category="HexGridDetails")
	int TurnNumber = 0;
	
	UPROPERTY(VisibleAnywhere, Category="HexGrid")
	TArray<FHexGridY> HexGrid2DArray;

	UPROPERTY(EditAnywhere, Category="BattleUnits")
	int RedTeamUnitsQuantity = 1;

	UPROPERTY(EditAnywhere, Category="BattleUnits")
	int BlueTeamUnitsQuantity = 1;

	UPROPERTY(EditAnywhere, Category="BattleUnits")
	TSubclassOf<ABattler> BattlerClass;

	UPROPERTY(EditAnywhere, Category="BattleRules")
	float BattleTickTimeStep = 1.0;

private:
	
	FTimerHandle BattleTimerHandle;

	UPROPERTY()
	TArray<ABattler*> BattlerArray;

};
