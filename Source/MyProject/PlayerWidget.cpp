// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UPlayerWidget::SetHelth(float cur_helth, float max)
{
	if (HelthBar) {
		HelthBar->SetPercent(cur_helth / max);
	}
}

void UPlayerWidget::SetText(float cur_helth)
{
	if (Helth_Text) {
		Helth_Text->SetText(FText::FromString(FString::SanitizeFloat(cur_helth)));
	}
}
