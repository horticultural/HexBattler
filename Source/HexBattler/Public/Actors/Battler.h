// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Battler.generated.h"


UENUM(BlueprintType)
enum ETeam
{
	Red     UMETA(DisplayName = "Red"),
	Blue      UMETA(DisplayName = "Blue"),
};

UENUM()
enum ETurnMode
{
	MoveToNextTile     UMETA(DisplayName = "MoveToNextTile"), // tried to call it move but there was a conflict with an engine file..
	Attack      UMETA(DisplayName = "Attack"),
};


UCLASS()
class HEXBATTLER_API ABattler : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ABattler();

	UFUNCTION(BlueprintNativeEvent)
	void Die();
	virtual void Die_Implementation();
	
	void Attack(int Seed, int TurnNumber, int BattlerNumber);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void UpdateHealth(int DeltaHealth);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Unit Info")
	TEnumAsByte<ETeam> Team;

	UPROPERTY(EditDefaultsOnly, Category="Unit Info")
	int TileRange = 5;

	UPROPERTY(EditAnywhere, Category="Unit Info")
	int Health = 30;

	UPROPERTY(EditAnywhere, Category="Unit Info")
	int TurnsPerAttack = 5;

	UPROPERTY(EditAnywhere, Category="Unit Info")
	int TurnsPerMove = 2;

	UPROPERTY(EditAnywhere, Category="Unit Info")
	int ActOnTurnNumber = 0;

	UPROPERTY(EditDefaultsOnly, Category="Unit Info")
	FIntVector2 DamageRange = FIntVector2(2,5);

	UPROPERTY(EditDefaultsOnly, Category="Unit Info")
	FIntVector2 TileLocation = FIntVector2(0,0);

	UPROPERTY(VisibleAnywhere, Category="Unit Info")
	ABattler* NearestEnemyBattler;

	UPROPERTY(VisibleAnywhere, Category="Unit Info")
	TEnumAsByte<ETurnMode> TurnMode;

	UPROPERTY()
	bool Alive = true;
	
};
