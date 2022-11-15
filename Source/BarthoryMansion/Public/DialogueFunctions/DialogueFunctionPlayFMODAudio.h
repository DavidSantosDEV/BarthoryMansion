// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Objects/DialogueFunction.h"
#include "DialogueFunctionPlayFMODAudio.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UDialogueFunctionPlayFMODAudio : public UDialogueFunction
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio")
	class UFMODEvent* FModAudio;

	virtual void Function(class UDialogueComponent* CallingComponent) override;//, UDialogueComponent* CallingComponent

};
