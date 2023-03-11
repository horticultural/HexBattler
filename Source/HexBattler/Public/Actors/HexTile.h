// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexTile.generated.h"


class UTextRenderComponent;

UCLASS()
class HEXBATTLER_API AHexTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexTile();
	
	void SetText(FText Text);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly)
	UTextRenderComponent* TextRenderComp;

};
