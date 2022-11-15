// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "IKAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UIKAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IK, meta = (BlueprintProtected = true))
	float LeftFootIK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IK, meta = (BlueprintProtected = true))
	float RightFootIK;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IK, meta = (BlueprintProtected = true))
	FRotator LeftFootRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IK, meta = (BlueprintProtected = true))
	FRotator RightFootRot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IK, meta = (BlueprintProtected = true))
	float Displacement;
public:

	UIKAnimInstance();

	void SetDisplacement(float newVal);

	void SetLeftFootIK(float IKValue);
	void SetRightFootIK(float IKValue);

	void SetRightFootRot(FRotator NewRot);
	void SetLeftFootRot(FRotator NewRot);
};
