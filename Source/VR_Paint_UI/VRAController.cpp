// Fill out your copyright notice in the Description page of Project Settings.


#include "VRAController.h"
#include "MotionControllerComponent.h"

#include "Stroke.h"

// Sets default values
AVRAController::AVRAController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionComponent"));
	SetRootComponent(MotionController);
	// For Oculus Quest must be setted with StaticMesh
	MotionController->SetShowDeviceModel(true);
	MotionController->SetDisplayModelSource(UMotionControllerComponent::CustomModelSourceId);

}

// Called when the game starts or when spawned
void AVRAController::BeginPlay()
{
	Super::BeginPlay();
}

void AVRAController::ButtonPressed()
{
	FTransform TR;
	TR.SetTranslation(GetActorLocation());
	CurrentStroke = GetWorld()->SpawnActor<AStroke>(StrokeToPaint,TR);
	CurrentStroke->SetOwner(this);
}

void AVRAController::ButtonRelease()
{
	CurrentStroke = nullptr;
}

// Called every frame
void AVRAController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentStroke != nullptr) // Only update if there is some stroke.
	{
		CurrentStroke->Update(GetActorLocation());
	}
}

void AVRAController::SetHand(FName Hand)
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
