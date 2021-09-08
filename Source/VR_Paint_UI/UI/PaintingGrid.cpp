// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingGrid.h"

void UPaintingGrid::AddPainting()
{
	if (PaintingGrid)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green,
			FString::Printf(TEXT("                               S'HA CRIDAT")));
	}
}
