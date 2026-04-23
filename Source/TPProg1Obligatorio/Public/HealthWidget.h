// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class TPPROG1OBLIGATORIO_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateBar(float Health, float MaxHealth);

protected:
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText;
};
