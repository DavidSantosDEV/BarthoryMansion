// Fropple Studios Copyright


#include "Objects/CustomImageDecorator.h"
#include "InputDetectionProcessor.h"
#include "Subsystems/InputChangeSubsystem.h"

const FSlateBrush* UCustomImageDecorator::FindImageBrush(FName TagOrId, bool bWarnIfMissing)
{
	const FCustomRichImageRow* ImageRow = FindCustomImageRow(TagOrId, bWarnIfMissing);
	if (ImageRow) {
		if (GEngine) {
			UInputChangeSubsystem* InputSubsystem = GEngine->GetEngineSubsystem<UInputChangeSubsystem>();
			if (InputSubsystem) {
				EInputDevice Device = InputSubsystem->GetCurrentInputDevice();
				if (Device != EInputDevice::Gamepad) {
					return &ImageRow->BrushPC;
				}
				else {
					return &ImageRow->BrushXbox;
				}
			}
		}
	}

	return nullptr;
}

FCustomRichImageRow* UCustomImageDecorator::FindCustomImageRow(FName TagOrId, bool bWarnIfMissing)
{
	if (CustomImageSet)
	{
		FString ContextString;
		return CustomImageSet->FindRow<FCustomRichImageRow>(TagOrId, ContextString, bWarnIfMissing);
	}

	return nullptr;
}
