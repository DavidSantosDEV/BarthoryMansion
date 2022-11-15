// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/PickableItemBase.h"
#include "Inventory/InventoryComponent.h"
#include "MyPlayerController.h"
#include "Characters/NPCCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyComponents/DialogueComponent.h"
#include "Characters/PlayerCharacter.h"

// Sets default values
APickableItemBase::APickableItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMesh"));
	RootComponent = ItemMesh;

	DialogueComp = CreateDefaultSubobject<UDialogueComponent>(FName("DialogueComp"));
}

void APickableItemBase::BeginPlay()
{
	Super::BeginPlay();
	DialogueComp->ChangeDialogueData(OnPickupDialogue);
	DialogueComp->OnStopDialogueEvent.AddUniqueDynamic(this, &APickableItemBase::StoppedTypingEvent);
}

void APickableItemBase::StoppedTypingEvent()
{
	AMyPlayerController* playerCasted = (AMyPlayerController*)UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(playerCasted) {
		playerCasted->SetInputToPlayer();
	}
}


void APickableItemBase::Interact_Implementation(AMyPlayerController* PlayerInstigating)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerInstigating->GetPawn());
	if (!Player) {
		return;
	}

	bool AddedItem = false;

	AddedItem = Player->GetInventoryComponent()->AddItem(Item);
	

	if (AddedItem) {
		if (bAddCachedDialogue) {
			if (NPCChar && DTdialogue) {
				NPCChar->GetDialogueComponent()->AddCachedDialogue(DTdialogue);
			}
		}

		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		bShouldExist = false;
		OnItemGivenDelegate.Broadcast(Player);
		if (DialogueComp->HasDialogue()) {
			PlayerInstigating->SetInputFocussedOn(this);
			DialogueComp->StartDialogue();
		}
		
	}
	//Destroy();
}

void APickableItemBase::BottomFaceButton_Implementation()
{
	DialogueComp->NextSentence();
}



