// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stroke.generated.h"

class USplineMeshComponent;
class UMaterialInterface;

UCLASS()
class VR_PAINT_UI_API AStroke : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStroke();

	void Update(FVector CursorLocation);
private:
	void CreateSpline(FVector StartPoint);

	UPROPERTY(VisibleAnywhere)
	TArray<USplineMeshComponent*> DynamicMeshArray;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* Mesh;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* Material;

	FVector LastLocation;
};
