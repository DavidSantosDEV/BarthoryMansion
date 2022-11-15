// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

UENUM(BlueprintType)
enum EInteractibleType {
	EPickup,
	EDialogue,
	EOther,
	EOneClick,
};


class AMyPlayerController;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BARTHORYMANSION_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.

public:
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Interaction")
	void SetIsInteractible(const bool bToggle);
	
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Interaction")
	bool GetIsInteractible();
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Interaction")
	bool GetNeedsHold();
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Interaction")
	float GetHoldTime();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Interaction")
	EInteractibleType GetInteractionType();

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category="Interaction")
	void Interact(AMyPlayerController* PlayerInstigating);
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Interaction")
	void StopInteract(AMyPlayerController* PlayerInstigating);
	UFUNCTION(BlueprintCallable,BlueprintNativeEvent, Category = "Interaction")
	void BackInteract(AMyPlayerController* PlayerInstigating);
};
