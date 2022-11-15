// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/InventoryComponent.h"
#include "SavingSystem/MansionSaveGame.h"
#include "Objects/MusicItem.h"
#include "Inventory/Item.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	OnInventoryUpdated.Broadcast();
	for (int i = 0; i < DefaultItems.Num(); ++i) 
	{
		AddItem(DefaultItems[i]);
	}
}

UItem* UInventoryComponent::CreateItem(TSubclassOf<UItem> ItemClass, bool bAddToInventory)
{
	if (IsValid(ItemClass)) {
		if(bAddToInventory && Items.Num() >= MaxItemCount) return nullptr;
		UItem* createdItem = (NewObject<UItem>(this, ItemClass));

		if (bAddToInventory) {
			AddItem(createdItem);
		}

		return createdItem;
	}
	return nullptr;
}

bool UInventoryComponent::AddItem(UItem* Item)
{
	if (Items.Num() >= MaxItemCount) return false;
	if (Item) {
		if (Items.Contains(Item)){
			return false;
		}
		Item->SetOwningInventory(this);
		Item->SetItemWorld(GetWorld());
		Items.AddUnique(Item);
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

bool UInventoryComponent::AddMusic(FMusicAndNames newMusic)
{
	if (newMusic.MusicEvent==nullptr || newMusic.NameOfMusic == "") {
		return false;
	}
	MusicItems.Add(newMusic);
	return true;
}

bool UInventoryComponent::RemoveItem(UItem* Item)
{
	if (Item) {
		Item->SetOwningInventory(nullptr);
		Item->SetItemWorld(nullptr);
		Items.RemoveSingle(Item); //.Remove
		OnInventoryUpdated.Broadcast();
		return true;
	}
	return false;
}

bool UInventoryComponent::RemoveItemOfClass(TSubclassOf<UItem> ItemClass)
{
	if (ItemClass) {
		int indexToRemove=0;
		for (indexToRemove = 0; indexToRemove < Items.Num(); ++indexToRemove) {
			UE_LOG(LogTemp, Warning, TEXT("%s"), Items[indexToRemove]->StaticClass());
			if (Items[indexToRemove]->GetClass() == ItemClass) {
				return RemoveItem(Items[indexToRemove]);
			}
		}
	}
	return false;
}

bool UInventoryComponent::HasItem(UItem* Item)
{
	if (Item) {
		for (int i = 0; i < Items.Num(); ++i) {
			if (Items[i]->StaticClass() == Item->StaticClass()) {
				return true;
			}
		}
	}
	return false;
}

bool UInventoryComponent::HasItemOfType(TSubclassOf<UItem> ItemType)
{
	if (IsValid(ItemType)) {
		for (int i = 0; i < Items.Num(); ++i) {
			if (Items[i]) {
				if (Items[i]->GetClass() == ItemType) {
					return true;
				}
			}
		}
	}
	return false;
}

void UInventoryComponent::SetItems(TArray<FMyItemData> newItems)
{
	for (int i = 0; i < Items.Num(); ++i) {
		if (Items[i]) {
			Items[i]->SetOwningInventory(nullptr);
			Items[i]->SetItemWorld(nullptr);
			Items.RemoveSingle(Items[i]);
		}
	}
	Items.Empty();
	for (int i = 0; i < newItems.Num(); ++i) {	
		//UItem* ItemToAdd = newItems[i].Item;
		if (!newItems[i].Item) {
			CreateItem(newItems[i].ItemClass, true);
		}
		else {
			AddItem(newItems[i].Item);
		}
		//AddItem(ItemToAdd);
	}
}

TArray<FMusicAndNames> UInventoryComponent::GetMusicItems()
{
	return MusicItems;
}

void UInventoryComponent::SetMusicItems(TArray<FMusicAndNames> setter)
{
	MusicItems = setter;
}
