// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UMyUserWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "Custom User Widget")
	void OnHidden(bool isLast);
};
