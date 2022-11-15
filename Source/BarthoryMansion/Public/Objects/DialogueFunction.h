// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DialogueFunction.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInLineNew, DefaultToInstanced)
class BARTHORYMANSION_API UDialogueFunction : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "DialogueFunction")
	virtual void ActivateFunction(class UDialogueComponent* CallingComponent);

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void OnActivatedFunction(UDialogueComponent* CallingComponent);

	virtual void Function(UDialogueComponent* CallingComponent) PURE_VIRTUAL(UDialogueFunction::Function,);//, UDialogueComponent* CallingComponent
};
