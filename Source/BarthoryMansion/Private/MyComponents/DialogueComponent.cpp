// Fill out your copyright notice in the Description page of Project Settings.

//#include "fmod_studio.hpp" //FMOD Library
#include "MyComponents/DialogueComponent.h"
#include "GameFramework/Actor.h" 
#include "Objects/DialogueFunction.h"
#include "Kismet/GameplayStatics.h"
#include "MyDeveloperSettings.h"
#include "FMODBlueprintStatics.h"
#include "MyHUD.h"


#define Context "Getting All Rows"
// Sets default values for this component's properties
UDialogueComponent::UDialogueComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bIsRepeating = false;

	//static ConstructorHelpers::FClassFinder<UFMODEvent> sTypeObj(TEXT("/Game/FMOD/Events/TypeUI.TypeUI"));
	//SoundType = sTypeObj.Object;
	//static ConstructorHelpers::FObjectFinder<UFMODEvent> sFinishObj(TEXT("/Game/FMOD/Events/FinishType.FinishType"));
	//SoundFinish = sFinishObj.Object;

	//Before I forget to even write this on the report
	/*
	
	Why is this hardcoded you ask?
	Well in short, epic games doesnt know how to deal with assets on their own engine that well, the TSoftObjectPtr was supposed to be the be-all-end-all solution,
	you can reference a ton of stuff using that right? Well yeah but also if you restart the editor magically the object although referenced will not reference the value
	will return a null ptr.
	The original idea was to use the developer settings class (which is for stuff like this) and to just reference the music assets using softobjectptrs 
	Notice a pattern? Its just like what im doing with the UI Icon prompts.... but UE4 doesnt love me enough to let this work.
	*/
}

void UDialogueComponent::BeginPlay()
{
	Super::BeginPlay();
	MyGameHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
}

void UDialogueComponent::Type()
{
	if (currentDataDialogue.Num()>0 && currentDataDialogue[Index_DialogueRows]->Text.IsValidIndex(Index_CharLine))
	{
		if (currentDataDialogue[Index_DialogueRows]->Text=="")
		{
			Index_DialogueRows++;
			//Mess more with this
			return;
		}
		//repeatingDialogue.Num()>0 ? repeatingDialogue[Index_DialogueRows] :
		FDialogue* CurrentRowData =  currentDataDialogue[Index_DialogueRows];
		if (MyGameHUD == nullptr)
		{
			StopDialogue();
			return;
		}
		TArray<TCHAR> thechar;
		char CheckChar = CurrentRowData->Text.GetCharArray()[Index_CharLine];

		if (CheckChar == '<') {
			//Find the end '>'
			do {
				char charAdd = CurrentRowData->Text.GetCharArray()[Index_CharLine];
				thechar.Add(charAdd);
				Index_CharLine++;
			} while (CurrentRowData->Text.GetCharArray()[Index_CharLine-1]!= '>');
		}
		thechar.Add(CurrentRowData->Text.GetCharArray()[Index_CharLine]);
		//Wtf is this shit???
		MyGameHUD->ApendDialogue(FString(thechar)); // To be deprecated

		MyGameHUD->AppendDialogue(FString(thechar));
		
		if (MyGameHUD->DialogueTextIsEqual(CurrentRowData->Text))
		{
			WhenPhraseEnds(CurrentRowData);
		}
		else
		{
			Index_CharLine++;
		}
	}
}

void UDialogueComponent::AddAwnsersToHUD()
{
	MyGameHUD->ClearAwnsers();
	for (int i = 0; i < CachedAwnsers.Num(); ++i)
	{
		MyGameHUD->AddAwnser(FDialogueAwnsers(CachedAwnsers[i]->Text, CachedAwnsers[i]->NextDialogue), this);
	}

	
	/*for (FDialogueAwnsers* awnser : CachedAwnsers) {
		MyGameHUD->AddAwnser(FDialogueAwnsers(awnser->Text,awnser->NextDialogue), this);
	}*/
}

bool UDialogueComponent::SetDialogueData()
{
	if (CachedDialogue.Num()) {
		TArray<FDialogue*> Rows;
		CachedDialogue[0]->GetAllRows(Context, Rows);
		if (Rows.Num()>0) {
			currentDataDialogue = Rows;
		}
		ClearCachedDialogueRow();
		bWasCachedDialogue = true;
		return true;
	}
	if (DTdialogue) {
		if (repeatingDialogue.Num() > 0) {
			bIsRepeating = true;
			currentDataDialogue = repeatingDialogue;
			repeatingDialogue.Empty();
			return true;
		}
		TArray<FDialogue*> AllRows;
		DTdialogue->GetAllRows(Context, AllRows);
		if (AllRows.Num()>0) {
			currentDataDialogue.Empty();
			currentDataDialogue = AllRows;
			bWasCachedDialogue = false;
			return true;
		}
		return false;
		
	}
	else {
		if (DefaultDialogue) {
			TArray<FDialogue*> AllRows;
			DefaultDialogue->GetAllRows(Context, AllRows);
			if (AllRows.Num() > 0) {
				currentDataDialogue.Empty();
				currentDataDialogue = AllRows;
				bWasCachedDialogue = false;
				return true;
			}
			return false;
		}
		return false;
	}
}

void UDialogueComponent::ClearCachedDialogueRow()
{
	if (CachedDialogue.Num()>=1) {
		CachedDialogue.RemoveAt(0);
	}
}

void UDialogueComponent::WhenPhraseEnds(FDialogue* CurrentRowData)
{
	GetWorld()->GetTimerManager().ClearTimer(MyTimerHandle);
	MyGameHUD->ToggleTypeLoop(false);	
	MyGameHUD->SetDialogue(CurrentRowData->Text);
	if (CurrentRowData->bIsRepeatable) {
		repeatingDialogue.Add(CurrentRowData);
	}	
	/*
	for (int i = 0; i < CurrentRowData->DialogueCommand.Num();++i)
	{
		if (CurrentRowData->DialogueCommand[i]) 
		{
			UDialogueFunction* func = (NewObject<UDialogueFunction>(this, CurrentRowData->DialogueCommand[i]));
			if (func) {
				func->ActivateFunction(this);
			}
		}
	}
	*/

	bCanGoNextPhrase = true;
	MyGameHUD->ToggleContinueButton(true);
}

bool UDialogueComponent::StartDialogue()
{
	//Index_DialogueList = (Index_DialogueList + 1 > DTdialogue.Num() - 1) ? 0 : Index_DialogueList + 1;
	Index_DialogueRows = 0;
	bCanGoNextPhrase = false;
	MyGameHUD->ToggleDialogueVisibility(true);
	MyGameHUD->SetDialogue("");
	MyGameHUD->ToggleContinueButton(false);
	if (SetDialogueData()) {
		StartTyping();
		return true;
	}
	else {
		StopDialogue();
		return false;
	}
	return false;
}

void UDialogueComponent::StartTyping()
{
	UE_LOG(LogTemp, Warning, TEXT("Started typing text"));
	MyGameHUD->ToggleContinueButton(false);
	MyGameHUD->SetDialoguePortrait(currentDataDialogue[Index_DialogueRows]->AvatarImage);
	Index_CharLine = 0;
	if (currentDataDialogue[Index_DialogueRows]->AwnsersList != nullptr) //If has question
	{
		CachedAwnsers.Empty();
		currentDataDialogue[Index_DialogueRows]->AwnsersList->GetAllRows(Context, CachedAwnsers);
	}
	GetWorld()->GetTimerManager().SetTimer(MyTimerHandle, this, &UDialogueComponent::Type, timeType, true, 0.f);

	for (int i = 0; i < currentDataDialogue[Index_DialogueRows]->DialogueCommand.Num(); ++i)
	{
		if (currentDataDialogue[Index_DialogueRows]->DialogueCommand[i])
		{
			UDialogueFunction* func = (NewObject<UDialogueFunction>(this, currentDataDialogue[Index_DialogueRows]->DialogueCommand[i]));
			if (func) {
				func->ActivateFunction(this);
			}
		}
	}

	MyGameHUD->ToggleTypeLoop(true);
}

/*TArray<FDialogue*> AllRows;
	static const FString Context("Getting All Rows");
	DTdialogue[Index_DialogueList]->GetAllRows(Context, AllRows);
	currentPhrase=AllRows*/
	//static const FString Context("");
	//FDialogue* diag = DTdialogue[CurrentDialogueIndex]->FindRow<FDialogue>(FName("Row_0"),Context, true);

void UDialogueComponent::NextSentence()
{
	if (bCanGoNextPhrase)
	{
		MyGameHUD->SetDialogue("");
		MyGameHUD->ToggleContinueButton(false);

		if (CachedAwnsers.Num() > 0) {
			//We have AWNSERS	
			MyGameHUD->ToggleDialogueVisibility(false);
			AddAwnsersToHUD();
			MyGameHUD->ToggleDialogueAwnsersVisibility(true);
			UE_LOG(LogTemp, Warning, TEXT("Awnsers are cached"));
		}
		else {
			if (currentDataDialogue.IsValidIndex(Index_DialogueRows+1))
			{
				Index_DialogueRows++;
				bCanGoNextPhrase = false;
				StartTyping();
			}
			else {
				StopDialogue();
			}
			
		}
	}
	else
	{
		WhenPhraseEnds(currentDataDialogue[Index_DialogueRows]);
	}
}

/*bool UDialogueComponent::HasNextSentence()
{
	if (CachedAwnsers.Num() > 0) {
		return true;
	}
	return (Index_DialogueRows < currentDataDialogue.Num() - 1) || bIsTyping;
}*/

void UDialogueComponent::StopDialogue()
{
	if (MyGameHUD) {
		MyGameHUD->ToggleDialogueVisibility(false);
		MyGameHUD->SetDialogue("");
		MyGameHUD->ToggleContinueButton(false);
	}
	if (OnStopDialogueEvent.IsBound()) {
		OnStopDialogueEvent.Broadcast();
	}
	
}

void UDialogueComponent::ChangeDialogueData(UDataTable* newDialogue)
{
	
	if (newDialogue && newDialogue->GetRowStruct() == FDialogue::StaticStruct()) {
		DTdialogue = newDialogue;
		ClearRepeatingDialogue();
	}
	//if (newDialogue->GetRowStruct()==FDialogue::StaticStruct()) {

	//}
}

void UDialogueComponent::ChangeDialogueDataByVal(TArray<FDialogue> NewDialogue)
{
}

void UDialogueComponent::ClearCachedAwnsers()
{
	CachedAwnsers.Empty();
	//MyGameHUD->ToggleDialogueVisibility(true);
	MyGameHUD->ToggleDialogueAwnsersVisibility(false);
	MyGameHUD->ClearAwnsers();
	StartDialogue();
}

void UDialogueComponent::ClearRepeatingDialogue()
{
	repeatingDialogue.Empty();
	bIsRepeating = false;
}

void UDialogueComponent::AddCachedDialogue(UDataTable* newCachedDialogue)
{
	if (newCachedDialogue && newCachedDialogue->GetRowStruct() == FDialogue::StaticStruct()) {
		
		CachedDialogue.Add(newCachedDialogue);

		/*TArray<FDialogue*> AllRows;
		newCachedDialogue->GetAllRows(Context, AllRows);
		CachedDialogue.Add(AllRows);*/
	}
	
}

void UDialogueComponent::SetCachedDialogue(TArray<UDataTable*> NewCached)
{
	CachedDialogue = NewCached;
}

void UDialogueComponent::SetWasRepeatingDialogue(bool IsRepeatingNow)
{
	bIsRepeating = IsRepeatingNow;
	repeatingDialogue.Empty();
	if (IsRepeatingNow) {
		TArray<FDialogue*> AllRows;
		DTdialogue->GetAllRows(Context, AllRows);
		for (int i = 0; i < AllRows.Num(); ++i) {
			if (AllRows[i]->bIsRepeatable) {
				repeatingDialogue.Add(AllRows[i]);
			}
		}
	}
}

bool UDialogueComponent::HasDialogue()
{
	if (DTdialogue) {
		return true;
	}
	else {
		return CachedDialogue.Num() > 0;
	}
	return false;
}

bool UDialogueComponent::GetIsRepeatingDialogue()
{
	return (repeatingDialogue.Num() > 0) || bIsRepeating;
}



