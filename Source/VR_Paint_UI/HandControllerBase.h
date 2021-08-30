// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandControllerBase.generated.h"

class UMotionControllerComponent;

UCLASS()
class VR_PAINT_UI_API AHandControllerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandControllerBase();

	virtual void ButtonPressed();
	virtual void ButtonRelease();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* MotionController;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* MeshLeft;
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* MeshRight;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetHand(FName Hand);
};
