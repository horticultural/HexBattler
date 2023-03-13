// Fill out your copyright notice in the Description page of Project Settings.


#include"HexBattler/Public/Actors/HexTile.h"

#include "Components/TextRenderComponent.h"

// Sets default values
AHexTile::AHexTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;
	TextRenderComp = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRenderComp"));
	TextRenderComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHexTile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHexTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//For debug and so I could ensure the grid was set out correctly in the visualisation.
void AHexTile::SetText(FText Text)
{
	TextRenderComp->SetText(Text);
}

