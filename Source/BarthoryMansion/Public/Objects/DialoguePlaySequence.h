// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Objects/DialogueFunction.h"
#include "MovieScene/Public/MovieSceneSequencePlayer.h"
#include "DialoguePlaySequence.generated.h"
/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UDialoguePlaySequence : public UDialogueFunction
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialogue Function")
	class ULevelSequence* SequenceToPlay;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Dialogue Function")
	FMovieSceneSequencePlaybackSettings SettingsPlayback;

protected:

	virtual void Function(UDialogueComponent* CallingComponent) override;
};
