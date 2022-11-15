// Fropple Studios Copyright


#include "Objects/DialogueFunctionRemoveItem.h"
#include "Kismet/GameplayStatics.h"
#include "Inventory/InventoryComponent.h"
#include "Characters/PlayerCharacter.h"

void UDialogueFunctionRemoveItem::Function(UDialogueComponent* CallingComponent)
{
	UWorld* const World = GetWorld();
	if (!ItemToRemove)return;
	if (World) {
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
		if (Player) {
			Player->GetInventoryComponent()->RemoveItemOfClass(ItemToRemove);
		}
	}
}
