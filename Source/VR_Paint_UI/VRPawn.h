// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "VRPawn.generated.h"

//class USceneComponent;
class UCameraComponent;
class AVRAController;

UCLASS()
class VR_PAINT_UI_API AVRPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVRPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	void RightTriggerPressed();
	void RightTriggerReleased();

	void Load();
	void Save();

protected:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* VRRoot;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

private:
	UPROPERTY(VisibleAnywhere)
	AVRAController* LeftController;

	UPROPERTY(VisibleAnywhere)
	AVRAController* RightController;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AVRAController> HandTypeController;
};
