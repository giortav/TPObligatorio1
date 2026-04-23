#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealNotificationWidget.generated.h"

UCLASS()
class TPPROG1OBLIGATORIO_API UHealNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ShowMessage(const FString& Message);

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* NotificationText;
};