// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HandControllerBase.h"
#include "UIPointerHandController.generated.h"

class UWidgetInteractionComponent;

UCLASS()
class VR_PAINT_UI_API AUIPointerHandController : public AHandControllerBase
{
	GENERATED_BODY()

public:
	AUIPointerHandController();

	virtual void ButtonPressed() override;
	virtual void ButtonRelease() override;

private:
	UPROPERTY()
	UWidgetInteractionComponent* WidgetInteract;
};
