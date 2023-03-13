// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Battler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDied);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamaged);

UENUM(BlueprintType)
enum ETeam
{
	Red     UMETA(DisplayName = "Red"),
	Blue      UMETA(DisplayName = "Blue"),
};

UENUM(BlueprintType)
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
	void InitTeam(ETeam Team);
	virtual void InitTeam_Implementation(ETeam Team);
	
	UFUNCTION()
	void Die();

	
	void Attack(int Seed, int TurnNumber, int BattlerNumber);

	UFUNCTION(BlueprintImplementableEvent)
	void StartAction(FVector TargetLocation, float DeltaTime);

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
	TEnumAsByte<ETeam> TeamName;

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Unit Info")
	TEnumAsByte<ETurnMode> TurnMode;

	UPROPERTY()
	bool Alive = true;

	UPROPERTY(BlueprintAssignable)
	FOnDied OnDied;

	UPROPERTY(BlueprintAssignable)
	FOnDamaged OnDamaged;
	
};
