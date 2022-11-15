// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Actors/ItemGiverWithDialogue.h"
#include "ItemGiverDoll.generated.h"

USTRUCT(BlueprintType)
struct FComponentWithDialogue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ANPCCharacter* CharacterWithComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	class UDataTable* NewDialogue;

	FComponentWithDialogue() {
		CharacterWithComp = nullptr;
		NewDialogue = nullptr;
	}
};

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API AItemGiverDoll : public AItemGiverWithDialogue
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditInstanceOnly, Category = "ItemGiver")
	TArray<FComponentWithDialogue> DialogueAndComps;

	virtual void GaveItem() override;
};
