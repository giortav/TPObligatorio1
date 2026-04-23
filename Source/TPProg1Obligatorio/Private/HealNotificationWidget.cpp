#include "HealNotificationWidget.h"
#include "Components/TextBlock.h"

void UHealNotificationWidget::ShowMessage(const FString& Message)
{
	if (NotificationText)
	{
		NotificationText->SetText(FText::FromString(Message));
	}
}