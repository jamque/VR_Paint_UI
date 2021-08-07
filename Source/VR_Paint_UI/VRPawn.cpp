// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"

#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "VRAController.h"

// Sets default values
AVRPawn::AVRPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	VRRoot = CreateDefaultSubobject<USceneComponent>(TEXT("VRRoot"));
	SetRootComponent(VRRoot);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(VRRoot);

}

// Called when the game starts or when spawned
void AVRPawn::BeginPlay()
{
	Super::BeginPlay();

	if (HandTypeController)
	{
		LeftController = GetWorld()->SpawnActor<AVRAController>(HandTypeController);
		if (LeftController != nullptr)
		{
			LeftController->AttachToComponent(VRRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			LeftController->SetOwner(this);
			LeftController->SetHand(TEXT("Left"));
		}
		RightController = GetWorld()->SpawnActor<AVRAController>(HandTypeController);
		if (RightController != nullptr)
		{
			RightController->AttachToComponent(VRRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			RightController->SetOwner(this);
			RightController->SetHand(TEXT("Right"));
		}
	}
}

