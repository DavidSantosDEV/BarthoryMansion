// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Application/IInputProcessor.h"

UENUM()
enum class EInputDevice //So, UE4 already has a enum for this "EInputDevices" 
						//but since "InputState.h" will cause a dependency that is not available on the Shipping version, i decided to create my own
{
	Keyboard,
	Gamepad,
};


DECLARE_DELEGATE_OneParam(FOnInputDeviceChangedDelegate, EInputDevice);
/**
 * Okay so the idea is that this will let me know when devices change, that way keyboard and gamepad can have different prompts on the go!
 * It was possible to do a check every single time the user has an input in gameplay but thats just filthy
 */
class InputDetectionProcessor : public IInputProcessor
{
private:
	EInputDevice CurrentInputDevice = EInputDevice::Keyboard;// = EInputDevices::Keyboard;
public:
	FOnInputDeviceChangedDelegate InputDeviceChangedDelegate;
public:
	void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override; //Welp its not usable but overriden all the same, interface requires it

	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override;

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;

	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;

	virtual bool HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp, const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent) override;

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
};
