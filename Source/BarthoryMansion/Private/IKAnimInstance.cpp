// Fropple Studios Copyright


#include "IKAnimInstance.h"

UIKAnimInstance::UIKAnimInstance()
{
}

void UIKAnimInstance::SetDisplacement(float newVal)
{
	Displacement = newVal;
}

void UIKAnimInstance::SetLeftFootIK(float IKValue)
{
	LeftFootIK = IKValue;
}

void UIKAnimInstance::SetRightFootIK(float IKValue)
{
	RightFootIK = IKValue;
}

void UIKAnimInstance::SetRightFootRot(FRotator NewRot)
{
	RightFootRot = NewRot;
}

void UIKAnimInstance::SetLeftFootRot(FRotator NewRot)
{
	LeftFootRot = NewRot;
}

