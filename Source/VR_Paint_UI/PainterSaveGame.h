// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PainterSaveGame.generated.h"

class AStroke;

USTRUCT()
struct FStrokeState
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AStroke> Class;

	UPROPERTY()
	TArray<FVector> ControlPoints;
};

/**
 * 
 */
UCLASS()
class VR_PAINT_UI_API UPainterSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	static UPainterSaveGame* Create();
	bool Save();
	static UPainterSaveGame* Load();

	void SerializeFromWorld(UWorld* World);
	void DeserializeToWorld(UWorld* World);

private:

	void ClearWorld(UWorld* World);

	UPROPERTY(VisibleAnywhere)
	TArray<FStrokeState> Strokes;
};
