// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Objects/DialogueFunction.h"
#include "GiveLanternFunction.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UGiveLanternFunction : public UDialogueFunction
{
	GENERATED_BODY()
public:
	virtual void Function(class UDialogueComponent* CallingComponent) override;
};
