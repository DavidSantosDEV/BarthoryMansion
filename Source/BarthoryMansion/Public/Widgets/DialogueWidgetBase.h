// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyComponents/DialogueComponent.h"
#include "DialogueWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UDialogueWidgetBase : public UUserWidget
{
	GENERATED_BODY()

	FText TextToType;
protected:
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "DialogueWidget")
	FText GetTextToType() { return TextToType; }

public:

	void SetText(FString newText);
	void AppendDialogue(FString CharAppend);

	UFUNCTION(BlueprintImplementableEvent)
	void ToggleContinueButton(const bool newVis);
	UFUNCTION(BlueprintImplementableEvent)
	void ToggleAwnsers(const bool newVis);
	UFUNCTION(BlueprintImplementableEvent)
	void AddAwnsers(FDialogueAwnsers Awnser);
	UFUNCTION(BlueprintImplementableEvent)
	void ClearAwnsers();
};
