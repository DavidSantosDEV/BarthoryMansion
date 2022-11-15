// Fropple Studios Copyright


#include "Actors/Gramophone.h"
#include "MyPlayerController.h"
#include "Characters/PlayerCharacter.h"
#include "Inventory/DiscItem.h"
#include "Objects/MusicItem.h"
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyComponents/DialogueComponent.h"

// Sets default values
AGramophone::AGramophone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	DialogueComp = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
}

void AGramophone::BeginPlay()
{
	Super::BeginPlay();

	DialogueComp->ChangeDialogueData(DTdialogue);

	DialogueComp->OnStopDialogueEvent.AddDynamic(this, &AGramophone::DialogueStopped);

}

void AGramophone::DialogueStopped()
{
	AMyPlayerController* _Player = Cast<AMyPlayerController>( UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (_Player) {
		_Player->SetInputToPlayer();
	}
}

void AGramophone::Interact_Implementation(AMyPlayerController* PlayerInstigating)
{
	if (PlayerInstigating) {

		APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerInstigating->GetPawn());
		if (Player) {
			MusicList = Player->GetInventoryComponent()->GetMusicItems();
			if (MusicList.Num()>0) {
				ShowGramUI();
			}
			else {
				if (DialogueComp->StartDialogue()) {
					PlayerInstigating->SetInputFocussedOn(this);
				}
			}
		}

	}
}

void AGramophone::BottomFaceButton_Implementation()
{
	DialogueComp->NextSentence();
}

