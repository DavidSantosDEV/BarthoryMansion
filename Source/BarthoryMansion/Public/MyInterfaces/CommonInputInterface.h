// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CommonInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommonInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class BARTHORYMANSION_API ICommonInputInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void OnEnteredFocus();
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void OnExitFocus();

	//Buttons
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void TopFaceButton();
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void RightFaceButton();
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void LeftFaceButton();
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void BottomFaceButton();

	//Ddpad
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void DPadUp();
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void DPadRight();
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void DPadLeft();
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void DPadDown();

	//Shoulder buttons/ Triggers
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void RightShoulderPad();
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void LeftShoulderPad();
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void RightTrigger();
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void LeftTrigger();

	//Analog
	UFUNCTION(BlueprintNativeEvent, Category="CommonInputInterface")
	void LeftAnalogForward(float Value);
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void LeftAnalogRight(float Value);
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void RightAnalogForward(float Value);
	UFUNCTION(BlueprintNativeEvent, Category = "CommonInputInterface")
	void RightAnalogRight(float Value);
	
};
