// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyComponents/DialogueComponent.h"
#include "MyHUD.generated.h"
/**
 * 
 */

class UItem;
class UDialogueWidgetBase;

UCLASS()
class BARTHORYMANSION_API AMyHUD : public AHUD
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, Category = "Widgets")
	TArray<class UUserWidget*> OpenedWidgets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UDialogueWidgetBase> DialogueWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "MyHUD")
	UDialogueWidgetBase* DialogueWidget;

public:

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void HideCurrentShowPrevious();

	//Inventory Related Stuff
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory")
	void ShowInventoryWidget();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory")
	bool GetInventoryVisibility();

	//Item inspection Stuff
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inspection")
	void StartInspectionItem(TSubclassOf<class AInspectableItem> ItemClass);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inspection")
	void StopInspection();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Inventory")
	void SetInventoryNeededItem(TSubclassOf<UItem> ItemClass, class AItemUsingInteractible* Interactible);
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, BlueprintCallable, Category = "Inventory")
	TSubclassOf<UItem> GetInventoryNeededItem();

	UFUNCTION(BlueprintImplementableEvent,BlueprintPure, BlueprintCallable, Category = "Inventory")
	UUserWidget* GetInventoryWidget();
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, BlueprintCallable, Category = "Inspection")
	UUserWidget* GetInspectionWidget();
	UFUNCTION(BlueprintImplementableEvent, BlueprintPure, BlueprintCallable, Category = "Inspection")
	UUserWidget* GetDialogueWidget();

	//Dialogue Related stuff
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleDialogueWidget(bool newVisibility);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleDialogueVisibility(bool Visibility);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleDialogueAwnsersVisibility(const bool Visibility);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ApendDialogue(const FString& charApend);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleTypeLoop(bool newToggle);

	UFUNCTION()
	void AppendDialogue(FString CharAppend);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowPauseMenu();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleViewUI(bool bToggle);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDialoguePortrait(UTexture2D* Avatar);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ToggleContinueButton(const bool Visibility);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDialogue(const FString& TextToSet);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddAwnser(const FDialogueAwnsers& TheAwnser, const UDialogueComponent* Comp);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ClearAwnsers();
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	bool DialogueTextIsEqual(const FString& ThecurrentPhrase);
};
