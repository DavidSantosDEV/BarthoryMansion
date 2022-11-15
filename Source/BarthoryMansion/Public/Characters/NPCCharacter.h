// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "MyInterfaces/InteractInterface.h"
#include "MyInterfaces/CommonInputInterface.h"
#include "NPCCharacter.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API ANPCCharacter : public ABaseCharacter, public IInteractInterface, public ICommonInputInterface
{
	GENERATED_BODY()

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraView;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (AllowPrivateAccess = "true"))
	class UDialogueComponent* DialogueComponent;

	AActor* PreviousViewTarget;
	AMyPlayerController* PlayerRef;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "NPC")
	bool bShouldExist = true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera")
	bool bBlendIn=false;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera")
	float fBlendExp;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera")
	float fBlendTime;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Camera")
	TEnumAsByte<EViewTargetBlendFunction> BlendFunc;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	bool bIsInteractible=true;
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	bool bNeedsHold = false;

	UPROPERTY(EditAnywhere, Category = Camera)
	float CameraBlendTime = 0.3f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Interaction")
	bool bChangeCamera = true;

	bool bIsTalking = false;

public:
	ANPCCharacter();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void ExtendInteract();
	virtual void ExtendStopInteract();

	UFUNCTION(BlueprintImplementableEvent)
	void InteractedStarting();
	UFUNCTION(BlueprintImplementableEvent)
	void InteractedEnding();

	UFUNCTION()
	void OnGameLoaded(class UMansionSaveGame* Save);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = NPC)
	UDialogueComponent* GetDialogueComponent() const { return DialogueComponent; }

#pragma region Interaction 
	//Interaction Interface
public:
	UFUNCTION()
	void StoppedTypingEvent();

	bool GetIsInteractible_Implementation();

	void SetIsInteractible_Implementation(const bool bToggle);

	bool GetNeedsHold_Implementation();

	void Interact_Implementation(AMyPlayerController* PlayerInstigating);

	void StopInteract_Implementation(AMyPlayerController* PlayerInstigating);

	void BackInteract_Implementation(AMyPlayerController* PlayerInstigating);
#pragma endregion
	//Common Input Interface
	void BottomFaceButton_Implementation();

	//Interaction
	UFUNCTION(BlueprintCallable, Category = "NPC")
	void SetShouldExist(bool bVal);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "NPC")
	bool GetShouldExist() { return bShouldExist; }

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "NPC")
	bool GetIsTalking() { return bIsTalking; }

	UFUNCTION(BlueprintCallable)
	void SetIsTalking(bool newVal);

};
