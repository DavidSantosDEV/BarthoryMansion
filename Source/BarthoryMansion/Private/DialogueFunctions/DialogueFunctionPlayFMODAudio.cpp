// Fropple Studios Copyright


#include "DialogueFunctions/DialogueFunctionPlayFMODAudio.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"

void UDialogueFunctionPlayFMODAudio::Function(UDialogueComponent* CallingComponent)
{
	if (!FModAudio)return;
	UWorld* const World = GetWorld();
	if (World) {
		UFMODBlueprintStatics::PlayEvent2D(World, FModAudio, true);
	}
}
