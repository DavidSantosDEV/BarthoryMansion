// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/Item.h"
#include "Inventory/InventoryComponent.h"
#include "Characters/BaseCharacter.h"

UItem::UItem() 
{
	ItemDisplayName = FText::FromString(TEXT("Name"));
	ItemDescription = FText::FromString(TEXT("Description"));
	ItemType = EItemType::EDoNothing;
	bRemoveAfterUse = true;
}

void UItem::UseItem(ABaseCharacter* Character)
{
	UE_LOG(LogTemp, Warning, TEXT("Used Item %s"), *GetName());
	OnUseItem(Character);
	if (bRemoveAfterUse) {
		OwningInventory->RemoveItem(this);
	}
}

void UItem::SetOwningInventory(UInventoryComponent* newInvent)
{
	OwningInventory = newInvent;
}

void UItem::SaveItemClass(TSubclassOf<UItem> ItemClass)
{
	MyClass = ItemClass;
}

void UItem::SetItemWorld(UWorld* newWorld)
{
	World = newWorld;
}
