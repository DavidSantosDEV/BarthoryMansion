// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractibleActorBase.h"
#include "PuzzleBase.generated.h"

/**
 * 
 */

UCLASS()
class BARTHORYMANSION_API APuzzleBase : public AInteractibleActorBase
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Puzzle");
	class APuzzleMaster* MyMaster;

	UFUNCTION()
	virtual void OnPuzzleComplete();

	virtual void SetIsComplete(bool bComplete) override;

public:
	void SetMaster(APuzzleMaster* newMaster);

};
