// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingPicker.h"
#include "Components/WidgetComponent.h"
#include "PaintingGrid.h"

// Sets default values
APaintingPicker::APaintingPicker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootActor = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootActor);

	PaintingGrid = CreateDefaultSubobject<UWidgetComponent>(TEXT("PaintingGrid"));
	PaintingGrid->SetupAttachment(GetRootComponent());
	ActionBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("ActionBar"));
	ActionBar->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void APaintingPicker::BeginPlay()
{
	Super::BeginPlay();

	UPaintingGrid* PG = Cast<UPaintingGrid>(PaintingGrid->GetUserWidgetObject());
	if (PG)
	{
		PG->AddPainting();
	}
}

