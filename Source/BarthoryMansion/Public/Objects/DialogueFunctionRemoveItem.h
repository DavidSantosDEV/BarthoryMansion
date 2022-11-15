// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Objects/DialogueFunction.h"
#include "Inventory/Item.h"
#include "DialogueFunctionRemoveItem.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UDialogueFunctionRemoveItem : public UDialogueFunction
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<UItem> ItemToRemove;

protected:

	virtual void Function(UDialogueComponent* CallingComponent) override;
};
