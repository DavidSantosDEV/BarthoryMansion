// Fropple Studios Copyright


#include "GiveLanternFunction.h"
#include "GameFramework/PlayerController.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void UGiveLanternFunction::Function(UDialogueComponent* CallingComponent)
{
	UWorld* const World = GetWorld();
	if (World) {
		const APlayerController* ControllingPC = UGameplayStatics::GetPlayerController(World, 0);
		if (ControllingPC) {
			APlayerCharacter* const Player = (APlayerCharacter*)ControllingPC->GetPawn();
			if (Player) {
				Player->SetCanUseLantern(true);
			}
		}
	}
}
