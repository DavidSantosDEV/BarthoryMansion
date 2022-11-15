// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Characters/NPCCharacter.h"
#include "CustomPlantNPC.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API ACustomPlantNPC : public ANPCCharacter
{
	GENERATED_BODY()
protected:
	virtual void ExtendInteract() override;
	virtual void ExtendStopInteract() override;
};
