// Fill out your copyright notice in the Description page of Project Settings.


#include "HandControllerBase.h"
#include "MotionControllerComponent.h"

// Sets default values
AHandControllerBase::AHandControllerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionComponent"));
	SetRootComponent(MotionController);
	// For Oculus Quest must be setted with StaticMesh
	MotionController->SetShowDeviceModel(true);
	MotionController->SetDisplayModelSource(UMotionControllerComponent::CustomModelSourceId);
}

void AHandControllerBase::ButtonPressed()
{
}

void AHandControllerBase::ButtonRelease()
{
}

// Called when the game starts or when spawned
void AHandControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHandControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHandControllerBase::SetHand(FName Hand)
{
	MotionController->SetTrackingMotionSource(Hand);
	if (Hand == FName(TEXT("Left")))
	{
		MotionController->SetCustomDisplayMesh(MeshLeft);
	}
	if (Hand == FName(TEXT("Right")))
	{
		MotionController->SetCustomDisplayMesh(MeshRight);
	}
}

