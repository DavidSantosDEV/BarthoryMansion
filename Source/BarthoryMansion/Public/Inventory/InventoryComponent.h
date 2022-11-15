// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SavingSystem/MansionSaveGame.h"
#include "Actors/Gramophone.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

class UItem;
class UMusicItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BARTHORYMANSION_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<UItem*> Items;
	TArray<FMusicAndNames> MusicItems;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int MaxItemCount = 12;
public:	
	// Sets default values for this component's properties
	UInventoryComponent();
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UItem* CreateItem(TSubclassOf<UItem>ItemClass, bool bAddToInventory);

	UFUNCTION(BlueprintCallable, Category="Inventory")
	bool AddItem(UItem* Item);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddMusic(FMusicAndNames newMusic);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(UItem* Item);
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItemOfClass(TSubclassOf<UItem> ItemClass);
	
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Inventory")
	bool HasItem(UItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	bool HasItemOfType(TSubclassOf<UItem> ItemType);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetItems(TArray<FMyItemData> newItems);

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "Inventory")
	TArray<UItem*> GetItems() { return Items; };

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Inventory")
	TArray<FMusicAndNames> GetMusicItems();

	void SetMusicItems(TArray<FMusicAndNames> setter);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Inventory")
	int GetMaxItemCount() { return MaxItemCount; };

	UPROPERTY(EditDefaultsOnly, Instanced, Category = "Inventory")
	TArray<UItem*> DefaultItems;

	UPROPERTY(VisibleAnywhere,BlueprintAssignable, Category = "Inventory")
	FOnInventoryUpdated OnInventoryUpdated;
};


