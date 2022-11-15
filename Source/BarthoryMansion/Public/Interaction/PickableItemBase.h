// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractibleActorBase.h"
#include "MyInterfaces/InteractInterface.h"
#include "MyComponents/DialogueComponent.h"
#include "MyInterfaces/CommonInputInterface.h"
#include "PickableItemBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemGiven, class APlayerCharacter*, PlayerChar);

UCLASS()
class BARTHORYMANSION_API APickableItemBase : public AInteractibleActorBase, public ICommonInputInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ItemMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	UDialogueComponent* DialogueComp;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Instanced, Category = Item)
	class UItem* Item;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Instanced, Category = Item)
	class UMusicItem* MusicItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	UDataTable* OnPickupDialogue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item)
	bool bAddCachedDialogue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Item, meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	class UDataTable* DTdialogue;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = Item)
	class ANPCCharacter* NPCChar;
public:	
	// Sets default values for this actor's properties
	APickableItemBase();

	virtual void BeginPlay();
protected:
	UFUNCTION()
	void StoppedTypingEvent();
public:	
	void Interact_Implementation(class AMyPlayerController* PlayerInstigating);

	
	void BottomFaceButton_Implementation();


	UPROPERTY(BlueprintAssignable, Category = "Item Pickup")
	FOnItemGiven OnItemGivenDelegate;
};
