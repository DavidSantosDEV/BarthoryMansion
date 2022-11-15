// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MusicItem.generated.h"

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInLineNew, DefaultToInstanced)
class BARTHORYMANSION_API UMusicItem : public UObject
{

	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item")
	FString ItemDisplayName;
	UPROPERTY(EditDefaultsOnly, Category = "Disc")
	class UFMODEvent* MusicToPlay;
public:
	UMusicItem();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetName() { return ItemDisplayName; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UFMODEvent* GetMusic() { return MusicToPlay; }
};
