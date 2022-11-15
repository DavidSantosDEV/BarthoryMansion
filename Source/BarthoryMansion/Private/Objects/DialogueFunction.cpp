// Fropple Studios Copyright


#include "Objects/DialogueFunction.h"

void UDialogueFunction::ActivateFunction(UDialogueComponent* CallingComponent)
{
	Function(CallingComponent);
	OnActivatedFunction(CallingComponent);
	ConditionalBeginDestroy();
}
