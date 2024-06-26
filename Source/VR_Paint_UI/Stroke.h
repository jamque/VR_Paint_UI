// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PainterSaveGame.h"
#include "Stroke.generated.h"

class UInstancedStaticMeshComponent;

UCLASS()
class VR_PAINT_UI_API AStroke : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStroke();

	void Update(FVector CursorLocation);

	FStrokeState SerializeToStruct() const;
	static AStroke* DeserializeFromStruct(UWorld* World, const FStrokeState& StrokeState);

private:
	void CreateSpline(FVector StartPoint);

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent * StrokeMeshInstanced;

	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* PelotasMeshInstanced;

	FVector LastLocation;
	TArray<FVector> Puntets;

	FTransform GetNextSegmentTransform(FVector Current) const;
	FTransform GetNextPelotasTransform(FVector Current) const;

	FVector GetNextSegmentScale(FVector Current) const;
	FQuat GetNextSegmentRotation(FVector Current) const;
	FVector GetNextSegmentLocation(FVector Current) const;



};
