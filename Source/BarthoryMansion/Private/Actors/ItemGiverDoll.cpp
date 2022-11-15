// Fropple Studios Copyright


#include "Actors/ItemGiverDoll.h"
#include "MyComponents/DialogueComponent.h"
#include "characters/NPCCharacter.h"

void AItemGiverDoll::GaveItem()
{
	for (int i = 0; i < DialogueAndComps.Num();++i) {
		if (DialogueAndComps[i].CharacterWithComp && DialogueAndComps[i].NewDialogue) {
			DialogueAndComps[i].CharacterWithComp->GetDialogueComponent()->ChangeDialogueData(DialogueAndComps[i].NewDialogue);
		}
	}
}
