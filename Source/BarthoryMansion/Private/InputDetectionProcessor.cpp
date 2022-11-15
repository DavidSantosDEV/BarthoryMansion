// Fill out your copyright notice in the Description page of Project Settings.


#include "InputDetectionProcessor.h"
#include "Engine/World.h" 
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

void InputDetectionProcessor::Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor)
{
}

bool InputDetectionProcessor::HandleAnalogInputEvent(FSlateApplication& SlateApp,
	const FAnalogInputEvent& InAnalogInputEvent)
{
	if (InAnalogInputEvent.GetKey().IsGamepadKey() && CurrentInputDevice != EInputDevice::Gamepad
		&& InAnalogInputEvent.GetAnalogValue() > 0.15f)
	{
		CurrentInputDevice = EInputDevice::Gamepad;
		InputDeviceChangedDelegate.ExecuteIfBound(CurrentInputDevice);
	}
	return false;
}

bool InputDetectionProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	if (InKeyEvent.GetKey().IsGamepadKey() && CurrentInputDevice != EInputDevice::Gamepad)
	{
		CurrentInputDevice = EInputDevice::Gamepad;
		InputDeviceChangedDelegate.ExecuteIfBound(CurrentInputDevice);
		return false;
	}
	if (!InKeyEvent.GetKey().IsGamepadKey() && CurrentInputDevice != EInputDevice::Keyboard)
	{
		CurrentInputDevice = EInputDevice::Keyboard;
		InputDeviceChangedDelegate.ExecuteIfBound(CurrentInputDevice);
		return false;
	}
	return false;
}

bool InputDetectionProcessor::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if (CurrentInputDevice != EInputDevice::Keyboard) {
		CurrentInputDevice = EInputDevice::Keyboard;
		InputDeviceChangedDelegate.ExecuteIfBound(CurrentInputDevice);
	}
	return false;
}

bool InputDetectionProcessor::HandleMouseWheelOrGestureEvent(FSlateApplication& SlateApp,
	const FPointerEvent& InWheelEvent, const FPointerEvent* InGestureEvent)
{
	if (CurrentInputDevice != EInputDevice::Keyboard)
	{
		CurrentInputDevice = EInputDevice::Keyboard;
		InputDeviceChangedDelegate.ExecuteIfBound(CurrentInputDevice);
	}
	return false;
}

bool InputDetectionProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if (CurrentInputDevice != EInputDevice::Keyboard)
	{
		CurrentInputDevice = EInputDevice::Keyboard;
		InputDeviceChangedDelegate.ExecuteIfBound(CurrentInputDevice);
	}
	return false;
}
