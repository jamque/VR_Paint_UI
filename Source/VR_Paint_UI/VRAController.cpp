// Fill out your copyright notice in the Description page of Project Settings.


#include "VRAController.h"
#include "MotionControllerComponent.h"

// Sets default values
AVRAController::AVRAController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionComponent"));
	SetRootComponent(MotionController);
	MotionController->bDisplayDeviceModel = true;

}

// Called when the game starts or when spawned
void AVRAController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVRAController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVRAController::SetHand(FName Hand)
{
	MotionController->SetTrackingMotionSource(Hand);
}
