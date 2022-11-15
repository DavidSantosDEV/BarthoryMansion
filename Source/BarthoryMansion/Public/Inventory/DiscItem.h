// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Inventory/Item.h"
#include "DiscItem.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UDiscItem : public UItem
{
	GENERATED_BODY()

	UDiscItem();
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Disc")
	class UFMODEvent* MusicToPlay;
public:

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Disc")
	UFMODEvent* GetMusic() { return MusicToPlay; }
};
