// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHealthWidget::UpdateBar(float Health, float MaxHealth)
{
	HealthProgressBar->SetPercent(Health / MaxHealth);

	if (HealthText)
	{
		FString HealthString = FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth);
		HealthText->SetText(FText::FromString(HealthString));
	}
}
