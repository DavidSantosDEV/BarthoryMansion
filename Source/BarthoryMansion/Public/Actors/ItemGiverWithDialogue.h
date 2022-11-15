// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterfaces/CommonInputInterface.h"
#include "ItemGiverWithDialogue.generated.h"

UCLASS()
class BARTHORYMANSION_API AItemGiverWithDialogue : public AActor, public ICommonInputInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Giver")
	class UBillboardComponent* ActorVisual;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Giver")
	class UDialogueComponent* DialogueComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	class UDataTable* DTdialogue;
	UPROPERTY(EditInstanceOnly, Category = "Item Giver")
	TSubclassOf<class UItem> ItemToGive;

	class AMyPlayerController* ControllerRef;
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDialogueFinished();

	UFUNCTION(BlueprintImplementableEvent)
	void OnGivenItem();

	virtual void GaveItem() PURE_VIRTUAL(&AItemGiverWithDialogue::GaveItem, );

public:	
	AItemGiverWithDialogue();

	UFUNCTION(BlueprintCallable, Category = "Item Giver")
	void GiveItemToPlayer();

	void BottomFaceButton_Implementation();
};
