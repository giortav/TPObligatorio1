#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealNotifierInterface.generated.h"

UINTERFACE(MinimalAPI)
class UHealNotifierInterface : public UInterface
{
	GENERATED_BODY()
};

class TPPROG1OBLIGATORIO_API IHealNotifierInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowHealMessage(float HealAmount);
};