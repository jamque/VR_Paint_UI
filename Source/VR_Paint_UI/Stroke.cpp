// Fill out your copyright notice in the Description page of Project Settings.


#include "Stroke.h"

#include "Components/SplineMeshComponent.h"

// Sets default values
AStroke::AStroke()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DynamicMeshArray.Empty();
	LastLocation = FVector(0.0f);
}

void AStroke::Update(FVector CursorLocation)
{
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
	USplineMeshComponent* DynamicMesh = NewObject<USplineMeshComponent>(this);
	DynamicMesh->SetMobility(EComponentMobility::Static);
	DynamicMesh->SetStaticMesh(Mesh);
	DynamicMesh->SetMaterial(0, Material);
	DynamicMesh->RegisterComponent();
	DynamicMesh->SetStartAndEnd(LastLocation, FVector(0.0f), StartPoint, FVector(0.0f));
	DynamicMesh->SetVisibility(true);
	DynamicMeshArray.Add(DynamicMesh);
}
