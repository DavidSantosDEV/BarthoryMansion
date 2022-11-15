// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
//#include "MyInterfaces/CommonInputInterface.h"
#include "MyPlayerController.generated.h"


UENUM()
enum EMyInputMode
{
	PlayerMode		UMETA(DisplayName = "Default"),
	ObjectMode		UMETA(DisplayName = "ObjectMode"),
	Cutscene		UMETA(DisplayName = "Cutscene"),
};


class UUserWidget;
class APlayerCharacter;
class AMyHUD;
/**
 *
 */
UCLASS()
class BARTHORYMANSION_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

	EMyInputMode currentInputMode = EMyInputMode::PlayerMode;

	class ASplineFollowCamera* NewCamera;
	ASplineFollowCamera* UsingCamera;


	UObject* currentInputObject;

	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "PlayerController")
	float velocitySizeSwitchCam = 120;

	FVector PreviousPawnDirection;

	APlayerCharacter* pMyPawn;
	AMyHUD* hMyHUD;
public:

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void SetUsingGamepad(bool bNewIsUsingGamepad);
	//UFUNCTION(BlueprintCallable, Category = "UI")
	//void SetMouseVisibility(bool bVisible);

	virtual void Tick(float deltaTime) override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="PlayerController")
	void SetInputFocussedOn(UObject* focussedObject);

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void EnableWidgetInput(TSubclassOf<UUserWidget> wWidget);
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void RemoveWidgetInput();
	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void SetInputToPlayer();

	UFUNCTION(BlueprintCallable, Category = "PlayerController")
	void SetInputToCutscene();

protected:
#pragma region InputHandling
	virtual void SetupInputComponent() override;

	void PauseGame();

	void TopFaceButton();
	void RightFaceButton();
	void LeftFaceButton();
	void BottomFaceButton();

	void DPadLeft();
	void DPadRight();

	void LeftThumbStickForward(float value);
	void LeftThumbStickRight(float value);
	
	void RightThumbStickForward(float value);
	void RightThumbStickRight(float value);

	void SelectButtonAction();
	void StartButtonAction();

	void RightTriggerAction();
	void LeftTriggerAction();

	void RightBumperAction();
	void LeftBumpAction();
#pragma endregion
public:

	void SetNewSplineCamera(ASplineFollowCamera* NewCam);

	class UCameraComponent* GetCurrentCamera();
};
