// Fill out your copyright notice in the Description page of Project Settings.


#include "UIPointerHandController.h"
#include "Components/WidgetInteractionComponent.h"
#include "InputCoreTypes.h"

AUIPointerHandController::AUIPointerHandController()
{
	WidgetInteract = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteract"));
	WidgetInteract->SetupAttachment(GetRootComponent());
	WidgetInteract->bShowDebug = true;
}

void AUIPointerHandController::ButtonPressed()
{
	WidgetInteract->PressPointerKey(EKeys::LeftMouseButton);
}

void AUIPointerHandController::ButtonRelease()
{
	WidgetInteract->ReleasePointerKey(EKeys::LeftMouseButton);
}
