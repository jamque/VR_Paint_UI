// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintBrushController.h"
#include "Stroke.h"

// Sets default values
APaintBrushController::APaintBrushController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APaintBrushController::BeginPlay()
{
	Super::BeginPlay();
}

void APaintBrushController::ButtonPressed()
{
	FTransform TR;
	TR.SetTranslation(GetActorLocation());
	CurrentStroke = GetWorld()->SpawnActor<AStroke>(StrokeToPaint,TR);
	CurrentStroke->SetOwner(this);
}

void APaintBrushController::ButtonRelease()
{
	CurrentStroke = nullptr;
}

// Called every frame
void APaintBrushController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentStroke != nullptr) // Only update if there is some stroke.
	{
		CurrentStroke->Update(GetActorLocation());
	}
}
