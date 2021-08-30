// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandControllerBase.h"
#include "PaintBrushController.generated.h"

class AStroke;

UCLASS()
class VR_PAINT_UI_API APaintBrushController : public AHandControllerBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APaintBrushController();

	void ButtonPressed() override;
	void ButtonRelease() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AStroke> StrokeToPaint;

	AStroke* CurrentStroke;

};
