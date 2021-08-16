// Fill out your copyright notice in the Description page of Project Settings.

#include "Stroke.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AStroke::AStroke()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	StrokeMeshInstanced = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Stroke Mesh"));
	StrokeMeshInstanced->SetupAttachment(Root);

	LastLocation = FVector(0.0f);
}

void AStroke::Update(FVector CursorLocation)
{
	/* Has lecture done*/
	CursorLocation = GetActorTransform().InverseTransformPosition(CursorLocation);
	/*****/
	if (LastLocation.IsZero())
	{
		LastLocation = CursorLocation;
		CreateSpline(CursorLocation);
	}
	// Create spline Mesh to manipulate (if you don't have it yet)
	if (FVector::Distance(LastLocation, CursorLocation) > 2.0f)
	{
		CreateSpline(CursorLocation);
		LastLocation = CursorLocation;
	}
	
	// Update endpoints (using new Cursor Location from Last Cursor Location)
	// Look last project how to create an Spline Mesh and setup Start and end position. Tangent now is irrebelant.
}

void AStroke::CreateSpline(FVector StartPoint)
{
	FTransform TR;
	TR.SetLocation(StartPoint);
	StrokeMeshInstanced->AddInstance(TR);
}
