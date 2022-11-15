// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CutsceneCameraActor.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API ACutsceneCameraActor : public ACameraActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera")
	FString UniqueCameraID;

	virtual void BeginPlay() override;

	virtual void BecomeViewTarget(class APlayerController* PC) override;
};
