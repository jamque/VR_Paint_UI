// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VRAController.generated.h"

class UMotionControllerComponent;

UCLASS()
class VR_PAINT_UI_API AVRAController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVRAController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetHand(FName Hand);

protected:
	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* MotionController;

	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* MeshLeft;
	UPROPERTY(EditDefaultsOnly)
	UStaticMesh* MeshRight;

};
