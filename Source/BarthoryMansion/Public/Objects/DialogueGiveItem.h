// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Objects/DialogueFunction.h"
#include "DialogueGiveItem.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UDialogueGiveItem : public UDialogueFunction
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly,Instanced, Category = "Item")
	class UItem* ItemToGive;

protected:

	virtual void Function(UDialogueComponent* CallingComponent) override;
};
