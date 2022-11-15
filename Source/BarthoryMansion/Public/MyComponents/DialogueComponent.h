// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h" 
#include "DialogueComponent.generated.h"

class USoundCue;
class UDataTable;
class UDialogueFunction;
class AMyHUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStoppedTyping);

USTRUCT(BlueprintType)
struct FDialogue : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue")
	FString Text;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue")
	USoundCue* sound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue")
	UAnimMontage* Montage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue")
	UTexture2D* AvatarImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=DialogueAwnsers"))
	UDataTable* AwnsersList; //Only base data table
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Dialogue")
	bool bIsRepeatable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue")
	TArray<TSubclassOf<UDialogueFunction>> DialogueCommand;

	FDialogue() {
		sound = nullptr;
		AvatarImage = nullptr;
		Montage = nullptr;
		AwnsersList = nullptr;
	}
};
USTRUCT(BlueprintType)
struct FDialogueAwnsers : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue")
	FString Text;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	UDataTable* NextDialogue; //Only base data table
	FDialogueAwnsers() {
		Text = "";
		NextDialogue = nullptr;
	}
	FDialogueAwnsers(FString AwnsText, UDataTable* dialog) {
		Text = AwnsText;
		NextDialogue = dialog;
	}
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BARTHORYMANSION_API UDialogueComponent : public UActorComponent
{
	GENERATED_BODY()
private:

	AMyHUD* MyGameHUD;
	FTimerHandle MyTimerHandle;

	TArray<FDialogue*> currentDataDialogue;
	
	TArray<FDialogue*> repeatingDialogue;

	TArray<FDialogueAwnsers*> CachedAwnsers;
	
	TArray<UDataTable*> CachedDialogue;
	

	int Index_DialogueRows = 0; //Index of Rows
	int Index_CharLine = 0; //Index of Chars

	bool bIsPlayingAnimation = false;
	bool bCanGoNextPhrase = false;
	bool bIsTyping = false;
	bool bIsRepeating = false;
	bool bWasCachedDialogue=false;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	UDataTable* DTdialogue; //Only base data table

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	UDataTable* DefaultDialogue;

	UPROPERTY(EditDefaultsOnly) //TBD make a global variable and use it for settings /Type speed FAST/NORMAL/SLOW
	float timeType = .1f;

public:
	//UPROPERTY(BlueprintAssignable)
	UPROPERTY(BlueprintAssignable)
	FStoppedTyping OnStopDialogueEvent;

private:
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StartTyping();

	UFUNCTION()
	void Type();

	void AddAwnsersToHUD();

	bool SetDialogueData();

	void ClearCachedDialogueRow();

	void WhenPhraseEnds(FDialogue* CurrentRowData);

public:
	UDialogueComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	bool StartDialogue();
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void NextSentence();
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void StopDialogue();
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ChangeDialogueData(UDataTable* newDialogue);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ChangeDialogueDataByVal(TArray<FDialogue> NewDialogue);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ClearCachedAwnsers();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void ClearRepeatingDialogue();

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void AddCachedDialogue(UDataTable* newDialogue);
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetCachedDialogue(TArray<UDataTable*> NewCached);

	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void SetWasRepeatingDialogue(bool IsRepeatingNow);

	bool HasDialogue();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Dialogue")
	bool GetIsRepeatingDialogue();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Dialogue")
	UDataTable* GetCurrentDialogue() { return DTdialogue; }

	//UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Dialogue")
	TArray<UDataTable*> GetCachedDialogue() { return CachedDialogue; }
};