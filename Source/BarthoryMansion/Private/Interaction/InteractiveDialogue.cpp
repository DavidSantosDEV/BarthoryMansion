// Fropple Studios Copyright


#include "Interaction/InteractiveDialogue.h"
#include "MyPlayerController.h"
#include "MyComponents/DialogueComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AInteractiveDialogue::AInteractiveDialogue()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("InteractionCollision"));
	CapsuleCollision->SetCollisionProfileName("OverlapAllDynamic");
	DialogueComp = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
}

void AInteractiveDialogue::BottomFaceButton_Implementation()
{
	DialogueComp->NextSentence();
}

void AInteractiveDialogue::Interact_Implementation(AMyPlayerController* PlayerInstigating)
{
	if (PlayerInstigating) {
		if (DialogueComp->HasDialogue()) {
			PlayerInstigating->SetInputFocussedOn(this);
			DialogueComp->StartDialogue();
		}
		
	}
}

EInteractibleType AInteractiveDialogue::GetInteractionType_Implementation()
{
	return EInteractibleType::EOneClick;
}

// Called when the game starts or when spawned
void AInteractiveDialogue::BeginPlay()
{
	Super::BeginPlay();
	if (DTdialogue) {
		DialogueComp->ChangeDialogueData(DTdialogue);
		DialogueComp->OnStopDialogueEvent.AddDynamic(this, &AInteractiveDialogue::EndDialogue);
	}
	
}

void AInteractiveDialogue::EndDialogue()
{
	AMyPlayerController* myPc = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (myPc) {
		myPc->SetInputToPlayer();
	}
}

