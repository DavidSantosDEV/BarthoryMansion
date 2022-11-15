// Fropple Studios Copyright

#include "FirstPersonTalkActor.h"
#include "MyPlayerController.h"
#include "MyComponents/DialogueComponent.h"
#include "MyHud.h"


AFirstPersonTalkActor::AFirstPersonTalkActor() : Super()
{
	DialogueComp = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComp"));
}

void AFirstPersonTalkActor::BeginPlay()
{
	Super::BeginPlay();
	DialogueComp->ChangeDialogueData(Dialogue);
}

void AFirstPersonTalkActor::StartTalking()
{
	DialogueComp->StartDialogue();
	bIsTalking = true;
}

void AFirstPersonTalkActor::Interact_Implementation(AMyPlayerController* PlayerInstigating)
{
	if (PlayerInstigating) {
		CurrentPlayer = PlayerInstigating;
		PreviousPlayerView = PlayerInstigating->GetViewTarget();
		PlayerInstigating->GetPawn()->SetActorHiddenInGame(true);
		PlayerInstigating->SetViewTargetWithBlend(this, BlendTime, BlendFunction, BlendExp, false);
		PlayerInstigating->SetInputFocussedOn(this);
	}
}

void AFirstPersonTalkActor::RightFaceButton_Implementation()
{
}

void AFirstPersonTalkActor::BottomFaceButton_Implementation()
{
	if(bIsTalking)
	DialogueComp->NextSentence();
}
