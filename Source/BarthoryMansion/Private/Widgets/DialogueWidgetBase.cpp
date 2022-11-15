// Fropple Studios Copyright


#include "Widgets/DialogueWidgetBase.h"

void UDialogueWidgetBase::SetText(FString newText)
{
	TextToType = FText::FromString(newText);
}

void UDialogueWidgetBase::AppendDialogue(FString CharAppend)
{
	//char test;
	FString textString = TextToType.ToString();
	textString.Append(CharAppend);
	//textString.AppendChar(CharAppend);
	TextToType = FText::FromString(textString);
}
