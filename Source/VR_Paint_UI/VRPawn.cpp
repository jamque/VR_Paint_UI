// Fill out your copyright notice in the Description page of Project Settings.


#include "VRPawn.h"

#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "VRAController.h"

#include "HeadMountedDisplayFunctionLibrary.h"

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

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);

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

void AVRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("RightTrigger"), EInputEvent::IE_Pressed, this, &AVRPawn::RightTriggerPressed);
	PlayerInputComponent->BindAction(TEXT("RightTrigger"), EInputEvent::IE_Released, this, &AVRPawn::RightTriggerReleased);
}

void AVRPawn::RightTriggerPressed()
{
	if (RightController != nullptr)
		RightController->ButtonPressed();
}

void AVRPawn::RightTriggerReleased()
{
	if (RightController != nullptr)
		RightController->ButtonRelease();
}

