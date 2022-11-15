// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractibleView.h"
#include "FirstPersonTalkActor.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API AFirstPersonTalkActor : public AInteractibleView
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "InteractionView")
	class UDialogueComponent* DialogueComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "StartDialogue")
	float TimeBeforeDialogue = 3.f;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly ,Category = "StartDialogue")
	class UDataTable* Dialogue;

	bool bIsTalking=false;

	AFirstPersonTalkActor();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void StartTalking();

public:
	void Interact_Implementation(AMyPlayerController* PlayerInstigating);

	void RightFaceButton_Implementation();

	void BottomFaceButton_Implementation();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "InteractionView")
	UDialogueComponent* GetDialogueComponent()const { return DialogueComp; };
};
