// Fropple Studios Copyright

#include "Objects/DialoguePlaySequence.h"
#include "LevelSequence/Public/LevelSequence.h" 
#include "LevelSequenceActor.h" 
#include "LevelSequencePlayer.h"



void UDialoguePlaySequence::Function(UDialogueComponent* CallingComponent)
{
	UWorld* const World = GetWorld();
	if (World) {
		ALevelSequenceActor* outActor;
		ULevelSequencePlayer::CreateLevelSequencePlayer(World, SequenceToPlay, SettingsPlayback, outActor);
		if (outActor) {
			outActor->SequencePlayer->Play();
		}
	}
	
	//FStringAssetReference SequenceName("/Game/NewLevelSequence.NewLevelSequence");
	//ULevelSequence* Asset = Cast<ULevelSequence>(SequenceName.TryLoad());
}
