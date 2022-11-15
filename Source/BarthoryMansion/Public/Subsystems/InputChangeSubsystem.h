// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "InputDetectionProcessor.h"
#include "InputChangeSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FChangeButtonLabelsDelegate, bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDynamicOnButtonChanged, bool,bGamepad);

UENUM()
enum class EGamepadIcon
{
	Xbox,
	Playstation,
	Nintendo,
};

class InputDetectionProcessor;
enum class EInputDevice;

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UInputChangeSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
private:
	TSharedPtr<InputDetectionProcessor> InputProcessor;
	EInputDevice CurrentInputDevice;
	EGamepadIcon CurrentSelectedIcons = EGamepadIcon::Playstation;
public:
	FChangeButtonLabelsDelegate InputDeviceChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Input Changed")
	FDynamicOnButtonChanged DynamicInputDeviceChangedDelegate;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void SetIconFamilyIndex(int iconIndex);

	void OnInputDeviceChanged(EInputDevice NewInputDevice);

	TSoftObjectPtr<UTexture2D> GetTextureForKey(FName InputName);

	UFUNCTION(BlueprintCallable, Category = "Input Subsystem")
	EInputDevice GetCurrentInputDevice() { return CurrentInputDevice; };
};
