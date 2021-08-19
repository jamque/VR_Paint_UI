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

	PelotasMeshInstanced = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Pelotas Mesh"));
	PelotasMeshInstanced->SetupAttachment(Root);

	LastLocation = FVector(0.0f);
}

void AStroke::Update(FVector CursorLocation)
{
	if (LastLocation.IsZero())
	{
		LastLocation = CursorLocation;
		PelotasMeshInstanced->AddInstance(GetNextPelotasTransform(CursorLocation));
		return;
	}
	// Create spline Mesh to manipulate (if you don't have it yet)
	//if (FVector::Distance(LastLocation, CursorLocation) > 0.4f)
	//{
		CreateSpline(CursorLocation);
		LastLocation = CursorLocation;
	//}
}

void AStroke::CreateSpline(FVector StartPoint)
{
	StrokeMeshInstanced->AddInstance(GetNextSegmentTransform (StartPoint));
	PelotasMeshInstanced->AddInstance(GetNextPelotasTransform(StartPoint));
}

FTransform AStroke::GetNextSegmentTransform(FVector Current) const
{
	FTransform SegmentTransform;

	SegmentTransform.SetScale3D(GetNextSegmentScale(Current));
	SegmentTransform.SetRotation(GetNextSegmentRotation(Current));
	SegmentTransform.SetLocation(GetNextSegmentLocation(Current));

	return SegmentTransform;
}

FTransform AStroke::GetNextPelotasTransform(FVector Current) const
{
	FTransform PT;
	PT.SetLocation(GetActorTransform().InverseTransformPosition(Current));
	return PT;
}

FVector AStroke::GetNextSegmentScale(FVector Current) const
{
	FVector Segment = Current - LastLocation;
	return FVector(Segment.Size(), 1.0f, 1.0f);
}

FQuat AStroke::GetNextSegmentRotation(FVector Current) const
{
	FVector Segment = Current - LastLocation;
	FVector SegmentNormal = Segment.GetSafeNormal();
	return FQuat::FindBetweenNormals(FVector::ForwardVector, SegmentNormal);
}

FVector AStroke::GetNextSegmentLocation(FVector Current) const
{
	return GetActorTransform().InverseTransformPosition(Current);
}
