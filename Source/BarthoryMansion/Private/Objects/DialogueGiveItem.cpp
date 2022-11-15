// Fropple Studios Copyright


#include "Objects/DialogueGiveItem.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/InventoryComponent.h"
#include "Characters/PlayerCharacter.h"

void UDialogueGiveItem::Function(UDialogueComponent* CallingComponent)
{
	UWorld* const World = GetWorld();
	if (World) {
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
		if (Player) {
			Player->GetInventoryComponent()->AddItem(ItemToGive);
			UE_LOG(LogTemp, Warning, TEXT("Gave Item to player"));
		}
	}
}
