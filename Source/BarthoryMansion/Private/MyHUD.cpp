// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Widgets/DialogueWidgetBase.h"
#include "MyPlayerController.h"

void AMyHUD::HideCurrentShowPrevious()
{
	if (OpenedWidgets.Num()>1){
		OpenedWidgets[1]->SetVisibility(ESlateVisibility::Visible);
		OpenedWidgets[0]->SetVisibility(ESlateVisibility::Hidden);
		OpenedWidgets.RemoveSingleSwap(OpenedWidgets[0], true);
		GetOwningPlayerController()->SetShowMouseCursor(true);
		//Keep Mouse showing
	}
	else {
		if (OpenedWidgets.IsValidIndex(0)) {
			OpenedWidgets[0]->SetVisibility(ESlateVisibility::Hidden);
			AMyPlayerController* myPlayerC = Cast<AMyPlayerController>(GetOwningPlayerController());
			if (myPlayerC) {
				FInputModeGameOnly InputMode;
				myPlayerC->SetInputMode(InputMode);
				myPlayerC->SetInputToPlayer();
				UWidgetBlueprintLibrary::SetFocusToGameViewport();
				myPlayerC->SetShowMouseCursor(false);
			}
			OpenedWidgets.RemoveSingleSwap(OpenedWidgets[0], true);
			//Hide Mouse if gamepad
		}
	}
}

void AMyHUD::AppendDialogue(FString CharAppend)
{
	if (DialogueWidget) {
		DialogueWidget->AppendDialogue(CharAppend);
	}
}
