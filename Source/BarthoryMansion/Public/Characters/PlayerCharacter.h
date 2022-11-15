// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerCharacter.generated.h"

class IInteractInterface;
class APlayerCameraManager;
class UWidgetComponent;
class UIKFootTracingComponent;

UCLASS()
class BARTHORYMANSION_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* InteractionCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	class UInventoryComponent* Inventory;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* WidgetInteract;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IK, meta = (AllowPrivateAccess = "true"))
	UIKFootTracingComponent* FootIKComp;

	class APlayerLantern* myLantern;

	class AMyPlayerController* myController;

public:

	APlayerCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Player Lantern")
	FName SocketHandLantern;
	UPROPERTY(EditDefaultsOnly, Category = "Player Lantern")
	FName SocketBackLantern;
	UPROPERTY(EditDefaultsOnly, Category = "Player Lantern")
	TSubclassOf<APlayerLantern> LanternClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Player")
	bool bHasLantern = false; //Lantern usage;
	UPROPERTY(BlueprintReadOnly, Category = "Player")
	bool bLanternOut = false;

	bool bCanIdle=true;

	virtual void BeginPlay();

#pragma region Input Handling
public:
	//Movement
	void MoveForward(float Value);
	void MoveRight(float Value);

	//Face Buttons
	void TopFaceButtonAction();
	void BottomFaceButtonAction();
	void LeftFaceButtonAction();
	void RightFaceButtonAction();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetCanUseLantern(bool bCanUse);

	UFUNCTION(BlueprintImplementableEvent, Category = "Player")
	void PlayLanternAnim();
	UFUNCTION(BlueprintImplementableEvent, Category = "Player")
	void StopAnimLantern();

	void CreateLantern();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void SetCanIdle(bool bNewVal);
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Player")
	bool GetCanIdle() { return bCanIdle; }

#pragma endregion

#pragma region Interaction
protected:
	//Interaction
	UObject* InteractibleSelected;

	UFUNCTION()
	void Interact();

	UFUNCTION(BlueprintImplementableEvent, Category="Interaction")
	void HighlightActor(AActor* TargetActor, const bool ToggleHighlight);

	UFUNCTION()
	void BeginOverlapInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlapInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:

	UFUNCTION(BlueprintCallable, Category = "Player")
	void CheckInteractibleInFront();

#pragma endregion

public:
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Player")
	FORCEINLINE class UInventoryComponent* GetInventoryComponent() const { return Inventory; }

	bool GetHasLanternOut() { return bLanternOut; }

	bool GetCanUseLantern() { return bHasLantern; }
};
