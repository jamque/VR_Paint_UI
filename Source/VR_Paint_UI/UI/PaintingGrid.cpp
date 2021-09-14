// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintingGrid.h"
#include "Components/SizeBox.h"

void UPaintingGrid::AddPainting()
{
	if (PaintingGrid)
	{
		if (GridCardClass)
		{
			UUserWidget* NewWid = CreateWidget<UUserWidget>(GetWorld(), GridCardClass);
			UWidget *Fill = PaintingGrid->GetChildAt(0);
			USizeBox *Caixa = Cast<USizeBox>(Fill);
			if (Caixa)
			{
				Caixa->AddChild(NewWid);
			}
		}
	}
}
