// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "InspectionCollisionComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSelectedThis);


class UWidgetComponent;
/**
 *  This is for me to cheat
 */
UCLASS(Blueprintable,meta = (BlueprintSpawnableComponent))
class BARTHORYMANSION_API UInspectionCollisionComponent : public USphereComponent
{
	GENERATED_BODY()

protected:
	UWidgetComponent* AttachedWidget;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Event Inspect")
	bool bIsOnlyOneTime=false;
	UPROPERTY(EditDefaultsOnly, Category = "Event Inspect")
	bool bIsEnabled = true;

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetIsEnabled() { return bIsEnabled; }

	UFUNCTION(BlueprintPure)
	UWidgetComponent* GetAttachedWidget();

	void SetAttachedWidget(UWidgetComponent* newWidget);

	UInspectionCollisionComponent();

	UFUNCTION(Category = "Event Inspect")
	void Press();

	UPROPERTY(VisibleAnywhere, BlueprintAssignable)
	FOnSelectedThis OnPlayerPressed;
};
