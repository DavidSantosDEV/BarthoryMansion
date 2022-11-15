// Fropple Studios Copyright


#include "SavingSystem/SaveGameActor.h"
#include "Subsystems/SavingSubsystem.h"
#include "MyPlayerController.h"
#include "Interaction/InteractibleActorBase.h"
#include "MyComponents/DialogueComponent.h"
#include "FMODBlueprintStatics.h"
#include "Kismet/GameplayStatics.h"

void ASaveGameActor::OnDialogueEnd()
{	
	AMyPlayerController* Player = (AMyPlayerController*)UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (Player) {
		Player->SetInputToPlayer();
	}
}

// Sets default values
ASaveGameActor::ASaveGameActor()
{
	PrimaryActorTick.bCanEverTick = false;

	DialogueComp = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComp"));

}

void ASaveGameActor::BeginPlay()
{
	Super::BeginPlay();
	DialogueComp->ChangeDialogueData(DialogueBase);
	DialogueComp->OnStopDialogueEvent.AddDynamic(this, &ASaveGameActor::OnDialogueEnd);
}

void ASaveGameActor::Interact_Implementation(AMyPlayerController* PlayerInstigating)
{
	if (!PlayerInstigating) {
		return;
	}
	USavingSubsystem* SavingSystemReference = GetGameInstance()->GetSubsystem<USavingSubsystem>();
	if (SavingSystemReference) {
		//Save state of things
		if (SavingSystemReference->SaveGameProgress()) {
			PlayAnimationSave();
			if (DialogueComp->StartDialogue()) {
				PlayerInstigating->SetInputFocussedOn(this);
			}
			if (SavingSound) {
				UFMODBlueprintStatics::PlayEvent2D(this, SavingSound, true);
			}
		}

	}
}

void ASaveGameActor::BottomFaceButton_Implementation()
{
	DialogueComp->NextSentence();
}

