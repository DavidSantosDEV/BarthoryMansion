// Fropple Studios Copyright


#include "MyPlayerController.h"
#include "MyHUD.h"
#include "MyInterfaces/CommonInputInterface.h"
#include "Characters/PlayerCharacter.h"
#include "Subsystems/InputChangeSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Actors/SplineFollowCamera.h"
#include "Kismet/GameplayStatics.h"
//#include "Blueprint/UserWidget.h" 

void AMyPlayerController::SetupInputComponent()
{
	
	Super::SetupInputComponent();
	InputComponent->BindAction("TopFaceButton", IE_Pressed, this, &AMyPlayerController::TopFaceButton);// .bExecuteWhenPaused = true;
	InputComponent->BindAction("RightFaceButton", IE_Pressed, this, &AMyPlayerController::RightFaceButton);// .bExecuteWhenPaused = true;
	InputComponent->BindAction("LeftFaceButton", IE_Pressed, this, &AMyPlayerController::LeftFaceButton);// .bExecuteWhenPaused = true;
	InputComponent->BindAction("BottomFaceButton", IE_Pressed, this, &AMyPlayerController::BottomFaceButton);// .bExecuteWhenPaused = true;

	InputComponent->BindAction("PauseToggle", IE_Pressed, this, &AMyPlayerController::PauseGame);

	InputComponent->BindAction("DPadRight", IE_Pressed, this, &AMyPlayerController::DPadRight);
	InputComponent->BindAction("DPadLeft", IE_Pressed, this, &AMyPlayerController::DPadLeft);

	InputComponent->BindAxis("LeftThumbStickForward", this, &AMyPlayerController::LeftThumbStickForward);
	InputComponent->BindAxis("LeftThumbStickRight", this, &AMyPlayerController::LeftThumbStickRight);

	InputComponent->BindAxis("RightThumbStickForward", this, &AMyPlayerController::RightThumbStickForward);
	InputComponent->BindAxis("RightThumbStickRight", this, &AMyPlayerController::RightThumbStickRight);
}

void AMyPlayerController::PauseGame()
{
	if(currentInputMode == PlayerMode) {
		hMyHUD->ShowPauseMenu();
	}
}

void AMyPlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	if (GetPawn()) {
		//UE_LOG(LogTemp, Warning, TEXT("Forward Vector: %s"), *GetPawn()->GetActorForwardVector().ToString())
		if (NewCamera && NewCamera != UsingCamera) {
			if (GetPawn()->GetVelocity().Size() <= velocitySizeSwitchCam || PreviousPawnDirection != GetPawn()->GetActorForwardVector()) {
				UE_LOG(LogTemp, Warning, TEXT("Velocity: %f PreviousDirection: %s CurrentDir: %s"),GetPawn()->GetVelocity().Size(),*PreviousPawnDirection.ToString(),*GetPawn()->GetVelocity().ToString())
				UsingCamera = NewCamera;
			}
		}
	}
	

}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	pMyPawn = Cast<APlayerCharacter>(GetPawn());
	hMyHUD = Cast<AMyHUD>(MyHUD);
	UInputChangeSubsystem* MyInputChangeSubsystem = GEngine->GetEngineSubsystem<UInputChangeSubsystem>();
	if (MyInputChangeSubsystem) {
		MyInputChangeSubsystem->InputDeviceChangedDelegate.AddUObject(this, &AMyPlayerController::SetUsingGamepad);
	}
}

void AMyPlayerController::SetUsingGamepad(bool bNewIsUsingGamepad)
{
	if (!bNewIsUsingGamepad && bShowMouseCursor) {
		bShowMouseCursor = !bNewIsUsingGamepad;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("%s"), bNewIsUsingGamepad ? "Gamepad" : "Keyboard");
}

/*/void AMyPlayerController::SetMouseVisibility(bool bVisible)
{
	if (bIsUsingGamepad) 
	{
		SetShowMouseCursor(false);
		UE_LOG(LogTemp, Warning, TEXT("Hidden"));
	}
	else 
	{
		SetShowMouseCursor(bVisible);
		UE_LOG(LogTemp, Warning, TEXT("%s"), bVisible ? "Visible" : "Hidden");
	}
}*/

void AMyPlayerController::SetInputFocussedOn(UObject* focussedObject)
{
	if (focussedObject->GetClass()->ImplementsInterface(UCommonInputInterface::StaticClass()))
	{
		GetPawn()->DisableInput(this);

		if (pMyPawn) {
			pMyPawn->SetCanIdle(false);
		}

		currentInputObject = focussedObject;
		currentInputMode = EMyInputMode::ObjectMode;
		//SetMouseVisibility(true);
	}
}

void AMyPlayerController::EnableWidgetInput(TSubclassOf<class UUserWidget> wWidget)
{

}

void AMyPlayerController::RemoveWidgetInput()
{

}

void AMyPlayerController::SetInputToPlayer()
{
	if (this) {
		currentInputMode = PlayerMode;
		GetPawn()->EnableInput(this);
		if (pMyPawn) {
			pMyPawn->SetCanIdle(true);
		}
		currentInputObject = nullptr;
	}
	//SetMouseVisibility(false);
}

void AMyPlayerController::SetInputToCutscene()
{
	currentInputObject = nullptr;
	currentInputMode = Cutscene;
	if (pMyPawn) {
		pMyPawn->SetCanIdle(false);
	}
}

void AMyPlayerController::TopFaceButton()
{
	switch (currentInputMode)
	{
	case EMyInputMode::PlayerMode:
		if (hMyHUD && pMyPawn) {
			hMyHUD->ShowInventoryWidget();
			
			//SetInputFocussedOn(hMyHUD->GetInventoryWidget(), true);
		}
		break;
	case EMyInputMode::ObjectMode:
		if (currentInputObject) {
			ICommonInputInterface::Execute_TopFaceButton(currentInputObject);
		}
		break;
	}
	
}

void AMyPlayerController::RightFaceButton()
{
	switch (currentInputMode)
	{
	case PlayerMode:
		if (pMyPawn) {
			pMyPawn->RightFaceButtonAction();
		}
		break;
	case ObjectMode:
		if (currentInputObject) {
			ICommonInputInterface::Execute_RightFaceButton(currentInputObject);
		}
		break;
	/*case ObjectUIMode:
		if (currentInputObject) {
			hMyHUD->HideCurrentShowPrevious();
		}*/
	default:
		break;
	}
}

void AMyPlayerController::LeftFaceButton()
{
	switch (currentInputMode)
	{
	case PlayerMode:
		if (pMyPawn) {
			pMyPawn->LeftFaceButtonAction();
		}
		break;
	case ObjectMode:
		if (currentInputObject) {
			ICommonInputInterface::Execute_LeftFaceButton(currentInputObject);
		}
		break;
	default:
		break;
	}
}

void AMyPlayerController::BottomFaceButton()
{
	switch (currentInputMode)
	{
	case PlayerMode:
		if (pMyPawn) {
			pMyPawn->BottomFaceButtonAction();
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("E"));

			}
		}
		break;
	case ObjectMode:
		if (currentInputObject) {
			ICommonInputInterface::Execute_BottomFaceButton(currentInputObject);
		}
		break;
	default:
		break;
	}
}

void AMyPlayerController::DPadLeft()
{
	switch (currentInputMode)
	{
	case PlayerMode:
		break;
	case ObjectMode:
		if (currentInputObject) {
			ICommonInputInterface::Execute_DPadLeft(currentInputObject);
		}
		break;
	default:
		break;
	}
}

void AMyPlayerController::DPadRight()
{
	switch (currentInputMode)
	{
	case PlayerMode:
		break;
	case ObjectMode:
		if (currentInputObject) {
			ICommonInputInterface::Execute_DPadRight(currentInputObject);
		}
		break;
	default:
		break;
	}
}

void AMyPlayerController::LeftThumbStickForward(float value)
{
	switch (currentInputMode)
	{
	case PlayerMode:
		if (pMyPawn) {
			pMyPawn->MoveForward(value);
		}
		break;
	case ObjectMode:
		if (currentInputObject) {
			ICommonInputInterface::Execute_LeftAnalogForward(currentInputObject, value);
		}
		break;
	default:
		break;
	}
}

void AMyPlayerController::LeftThumbStickRight(float value)
{
	switch (currentInputMode)
	{
	case PlayerMode:
		if (pMyPawn) {
			pMyPawn->MoveRight(value);
		}
		break;
	case ObjectMode:
		if (currentInputObject) {
			ICommonInputInterface::Execute_LeftAnalogRight(currentInputObject, value);
			//ICommonInputInterface::Execute_BottomFaceButton(currentInputObject);
		}
		break;
	default:
		break;
	}
}

void AMyPlayerController::RightThumbStickForward(float value)
{
}

void AMyPlayerController::RightThumbStickRight(float value)
{
}

void AMyPlayerController::SelectButtonAction()
{
}

void AMyPlayerController::StartButtonAction()
{
}

void AMyPlayerController::RightTriggerAction()
{
}

void AMyPlayerController::LeftTriggerAction()
{
}

void AMyPlayerController::RightBumperAction()
{
}

void AMyPlayerController::LeftBumpAction()
{
	
}

void AMyPlayerController::SetNewSplineCamera(ASplineFollowCamera* NewCam)
{
	if (NewCam) {
		NewCamera = NewCam;
		if (GetPawn()) {
			PreviousPawnDirection = GetPawn()->GetActorForwardVector();
		}
	}

}

UCameraComponent* AMyPlayerController::GetCurrentCamera()
{
	if (UsingCamera) {
		return UsingCamera->GetCameraComponent();
	}
	return nullptr;
}
