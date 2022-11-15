// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include "Engine/DataTable.h" 
#include "Item.generated.h"

/**
 * 
 */

/*USTRUCT(BlueprintType)
struct FItemInfo : public FTableRowBase
{
	GENERATED_BODY()


};*/

UENUM(BlueprintType)
enum EItemType {
	EDoNothing,
	EUsable,
	EInspectable,
};


class AInspectableItem;

UCLASS(Abstract, BlueprintType, Blueprintable, EditInLineNew, DefaultToInstanced)
class BARTHORYMANSION_API UItem : public UObject
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(Transient)
	class UWorld* World;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FText ItemDisplayName;
	UPROPERTY(EditDefaultsOnly, Category = "Item", meta = (Multiline = true))
	FText ItemDescription;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	class UTexture2D* ItemThumbnail;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TSubclassOf<AInspectableItem> InspectionObject;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	TEnumAsByte<EItemType> ItemType;
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	bool bRemoveAfterUse;

	UPROPERTY()
	class UInventoryComponent* OwningInventory;

	UPROPERTY()
	TSubclassOf<UItem> MyClass;

public:
	UItem();
	//virtual void Use(class ABaseCharacter* Character) PURE_VIRTUAL(UItem, );
	UFUNCTION(BlueprintImplementableEvent, Category = "Item")
	void OnUseItem(class ABaseCharacter* Character);

	virtual class UWorld* GetWorld() const { return World; };

	UFUNCTION(BlueprintCallable, Category = "Item")
	virtual void UseItem(ABaseCharacter* Character);
	
	void SetOwningInventory(UInventoryComponent* newInvent);

	void SaveItemClass(TSubclassOf<UItem> ItemClass);
	
	void SetItemWorld(UWorld* newWorld);
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	FORCEINLINE FText GetItemName() { return ItemDisplayName; };

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	FORCEINLINE FText GetDescription() { return ItemDescription; };

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	FORCEINLINE UTexture2D* GetThumbnail() { return ItemThumbnail; };

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	FORCEINLINE TSubclassOf<AInspectableItem> GetInspectionableItem() { return InspectionObject; };

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	FORCEINLINE TEnumAsByte<EItemType> GetItemType() { return ItemType; };

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	FORCEINLINE UInventoryComponent* GetOwningInventory() { return OwningInventory; };

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Item")
	FORCEINLINE TSubclassOf<UItem> GetItemClass() { return MyClass; };
};
