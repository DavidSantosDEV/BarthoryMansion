// Fropple Studios Copyright

#pragma once

#include "MyInterfaces/InteractInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TogglePuzzleTorch.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleWasToggled);

UCLASS()
class BARTHORYMANSION_API ATogglePuzzleTorch : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATogglePuzzleTorch();

protected:

	bool bIsActive=true;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly, Category = "Puzzle")
	bool bStartToggled;
	
	bool bIsToggled = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Puzzle")
	TArray<ATogglePuzzleTorch*> ActorsToToggle;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnToggleUpdateVisuals();

public:	

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetStateAsOriginal();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SetToggleAsComplete();

	void Toggle();

	void ToggleDescrete();

	UPROPERTY(BlueprintAssignable)
	FPuzzleWasToggled OnToggleDelegate;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Puzzle")
	bool GetIsToggled() { return bIsToggled; };

	UFUNCTION(BlueprintCallable)
	void SetPuzzleActive(bool bNewActive) { bIsActive = bNewActive; };

	bool GetIsInteractible_Implementation();

	EInteractibleType GetInteractionType_Implementation();

	void Interact_Implementation(class AMyPlayerController* PlayerInstigating);
};
