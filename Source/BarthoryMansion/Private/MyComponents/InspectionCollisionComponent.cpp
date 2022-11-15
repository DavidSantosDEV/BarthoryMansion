// Fropple Studios Copyright


#include "MyComponents/InspectionCollisionComponent.h"


UWidgetComponent* UInspectionCollisionComponent::GetAttachedWidget()
{
	return AttachedWidget;
}

void UInspectionCollisionComponent::SetAttachedWidget(UWidgetComponent* newWidget)
{
	AttachedWidget = newWidget;
}

UInspectionCollisionComponent::UInspectionCollisionComponent() : Super()
{
	SetCollisionProfileName(FName("InspectableOnly"), false);
	SetGenerateOverlapEvents(false);
}

void UInspectionCollisionComponent::Press()
{
	if (bIsEnabled) {
		OnPlayerPressed.Broadcast();
		bIsEnabled = !bIsOnlyOneTime;
	}
	
}
