// Fropple Studios Copyright


#include "Actors/ItemGiverWithDialogue.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "Characters/PlayerCharacter.h"
#include "Inventory/InventoryComponent.h"
#include "Components/BillboardComponent.h"
#include "MyComponents/DialogueComponent.h"

void AItemGiverWithDialogue::BeginPlay()
{
	Super::BeginPlay();
	DialogueComp->OnStopDialogueEvent.AddDynamic(this, &AItemGiverWithDialogue::OnDialogueFinished);
}

void AItemGiverWithDialogue::OnDialogueFinished()
{
	if (ControllerRef) {
		ControllerRef->SetInputToPlayer();
	}
}

// Sets default values
AItemGiverWithDialogue::AItemGiverWithDialogue()
{
	PrimaryActorTick.bCanEverTick = false;

	ActorVisual = CreateDefaultSubobject<UBillboardComponent>(TEXT("Actor Visuals"));
	RootComponent = ActorVisual;

	DialogueComp = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
}

void AItemGiverWithDialogue::GiveItemToPlayer()
{
	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerController) {
		APlayerCharacter* Player = (APlayerCharacter*)PlayerController->GetPawn();
		if (Player) {

			Player->GetInventoryComponent()->CreateItem(ItemToGive, true);
			
			if (DTdialogue) {
				DialogueComp->ChangeDialogueData(DTdialogue);
				DialogueComp->StartDialogue();
				PlayerController->SetInputFocussedOn(this);
				ControllerRef = PlayerController;
			}
			OnGivenItem();
			GaveItem();
		}
	}

}

void AItemGiverWithDialogue::BottomFaceButton_Implementation()
{
	DialogueComp->NextSentence();
}

