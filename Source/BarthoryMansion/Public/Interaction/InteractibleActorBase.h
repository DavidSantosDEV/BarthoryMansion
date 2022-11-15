// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterfaces/InteractInterface.h"
#include "InteractibleActorBase.generated.h"

UCLASS(Abstract)
class BARTHORYMANSION_API AInteractibleActorBase : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractibleActorBase();

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Interactible")
	bool bShouldExist=true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactible")
	bool bIsInteractible;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Interactible")
	bool bIsCompleted=false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interactible")
	bool bNeedsHold;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interactible")
	float fHoldTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interactible")
	TEnumAsByte<EInteractibleType> InteractibleType;

	UFUNCTION()
	void OnGameLoadedSave(class UMansionSaveGame* Save);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnSetShouldExist(bool bNewShouldExist);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnSetInteractible(bool bNewInt);

public:
	//Load Data when shows up
	virtual void BeginPlay() override;
	//Save temp data
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReaso) override;

	bool GetIsInteractible_Implementation();

	void SetIsInteractible_Implementation(const bool bToggle);

	FORCEINLINE bool GetNeedsHold_Implementation() { return bNeedsHold; };

	FORCEINLINE float GetHoldTime_Implementation() { return fHoldTime; };

	FORCEINLINE EInteractibleType GetInteractionType_Implementation() { return InteractibleType; };

	void SetPreviousData(const bool ShouldExist, const bool IsInteractible);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void SetIsComplete(bool bComplete);

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
	void OnSetIsComplete(bool newIsComplete);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	virtual void SetShouldExist(bool newShouldExist);

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Interaction")
	FORCEINLINE bool GetShouldExist() { return bShouldExist; };
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Interaction")
	FORCEINLINE bool GetIsComplete() { return bIsCompleted; };
};
