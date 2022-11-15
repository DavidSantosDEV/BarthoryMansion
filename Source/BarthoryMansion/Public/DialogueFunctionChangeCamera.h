// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Objects/DialogueFunction.h"
#include "DialogueFunctionChangeCamera.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UDialogueFunctionChangeCamera : public UDialogueFunction
{
	GENERATED_BODY()
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "DialogueFunction")
	FString CameraID;

	virtual void Function(UDialogueComponent* CallingComponent) override;
};
