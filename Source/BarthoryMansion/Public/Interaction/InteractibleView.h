// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "MyInterfaces/CommonInputInterface.h"
#include "Interaction/InteractibleActorBase.h"
#include "InteractibleView.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractionViewDel);

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API AInteractibleView : public AInteractibleActorBase, public ICommonInputInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractibleView", meta = (AllowPrivateAccess = "true"))
	USceneComponent* InteractionRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractibleView", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* InteractionCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "InteractibleView", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraView;

	AActor* PreviousPlayerView;
	AMyPlayerController* CurrentPlayer;

	AInteractibleView();

protected:
	UPROPERTY(EditInstanceOnly,BlueprintReadOnly, Category = "Camera Blend")
	float BlendTime=1.f;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera Blend")
	TEnumAsByte<EViewTargetBlendFunction> BlendFunction = EViewTargetBlendFunction::VTBlend_EaseIn;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera Blend")
	float BlendExp=0.f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "InteractibleView")
	float CameraMaxYaw = 50.f;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "InteractibleView")
	float CameraMaxPitch = 30.f;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "InteractibleView")
	float InterpSpeed = 0.98f;

public:

	UPROPERTY(VisibleAnywhere,BlueprintAssignable)
	FInteractionViewDel onInteractedViewBegin;

	void Interact_Implementation(AMyPlayerController* PlayerInstigating);
	
	void StopInteract_Implementation(AMyPlayerController* PlayerInstigating);
	
	void BackInteract_Implementation(AMyPlayerController* PlayerInstigating);


	void LeftAnalogForward_Implementation(float Value);
	
	void LeftAnalogRight_Implementation(float Value);

	void RightFaceButton_Implementation();
};
