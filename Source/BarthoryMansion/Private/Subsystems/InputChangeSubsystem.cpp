// Fropple Studios Copyright


#include "Subsystems/InputChangeSubsystem.h"
#include "MyDeveloperSettings.h"
#include "InputDetectionProcessor.h"

void UInputChangeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	CurrentInputDevice = EInputDevice::Keyboard;
	//TODO LOAD PROFILE <-
	CurrentSelectedIcons = EGamepadIcon::Xbox;

	if (FSlateApplication::IsInitialized())
	{
		InputProcessor = MakeShared<InputDetectionProcessor>();
		FSlateApplication& SlateApp = FSlateApplication::Get();
		SlateApp.RegisterInputPreProcessor(InputProcessor);
		InputProcessor->InputDeviceChangedDelegate.BindUObject(this, &UInputChangeSubsystem::OnInputDeviceChanged);
		//InputProcessor->InputDeviceChangedDelegate.BindRaw(this, &UInputChangeSubsystem::OnInputDeviceChanged);
		InputDeviceChangedDelegate.Broadcast(CurrentInputDevice == EInputDevice::Gamepad);
		DynamicInputDeviceChangedDelegate.Broadcast(CurrentInputDevice == EInputDevice::Gamepad);
	}
}

void UInputChangeSubsystem::Deinitialize()
{
	if (FSlateApplication::IsInitialized() && InputProcessor.IsValid())
	{
		FSlateApplication& SlateApp = FSlateApplication::Get();
		SlateApp.UnregisterInputPreProcessor(InputProcessor);
		InputProcessor.Reset();
	}
}

void UInputChangeSubsystem::SetIconFamilyIndex(int iconIndex)
{
	EGamepadIcon SelectedIcons;
	switch (iconIndex) {
	default:
	case 0:
		SelectedIcons = EGamepadIcon::Xbox;
		break;
	case 1:
		SelectedIcons = EGamepadIcon::Playstation;
		break;
	case 2:
		SelectedIcons = EGamepadIcon::Nintendo;
		break;
	}
	if (CurrentSelectedIcons != SelectedIcons) {
		CurrentSelectedIcons = SelectedIcons;
		InputDeviceChangedDelegate.Broadcast(true);
		DynamicInputDeviceChangedDelegate.Broadcast(true);
	}
}

void UInputChangeSubsystem::OnInputDeviceChanged(EInputDevice NewInputDevice)
{
	CurrentInputDevice = NewInputDevice;
	InputDeviceChangedDelegate.Broadcast(NewInputDevice == EInputDevice::Gamepad);
	DynamicInputDeviceChangedDelegate.Broadcast(NewInputDevice == EInputDevice::Gamepad);
	UE_LOG(LogTemp, Warning, TEXT("Using Gamepad : %s"), (NewInputDevice == EInputDevice::Gamepad) ? TEXT("Yes") : TEXT("No"));
}

TSoftObjectPtr<UTexture2D> UInputChangeSubsystem::GetTextureForKey(FName InputName)
{
	const UMyDeveloperSettings* DevSettings = GetDefault<UMyDeveloperSettings>();
	if (DevSettings) {
		const FInputToIcon* data = DevSettings->InputIconMap.Find(InputName);
		if (data != nullptr) {
			//FInputToIcon& data = ;
			if (CurrentInputDevice == EInputDevice::Gamepad)
			{
				switch (CurrentSelectedIcons) {
				case EGamepadIcon::Playstation:
					if (data->IconPS) {
						return data->IconPS;
					}
					return data->IconXbox;

				case EGamepadIcon::Nintendo:
					if (data->IconNintendo) {
						return data->IconNintendo;
					}
					return data->IconXbox;


				default:
					return data->IconPS;
				}
			}
			else
			{
				return data->IconKeyboard;
			}
		}
	}
	return nullptr;
}
