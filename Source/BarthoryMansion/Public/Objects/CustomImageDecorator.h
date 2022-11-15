// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlockImageDecorator.h"
#include "CustomImageDecorator.generated.h"


USTRUCT(Blueprintable, BlueprintType)
struct BARTHORYMANSION_API FCustomRichImageRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Appearance)
	FSlateBrush BrushPC;
	UPROPERTY(EditAnywhere, Category = Appearance)
	FSlateBrush BrushXbox;
	UPROPERTY(EditAnywhere, Category = Appearance)
	FSlateBrush BrushPS;
	UPROPERTY(EditAnywhere, Category = Appearance)
	FSlateBrush BrushNintendo;
};
/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UCustomImageDecorator : public URichTextBlockImageDecorator
{
	GENERATED_BODY()
public:
	virtual const FSlateBrush* FindImageBrush(FName TagOrId, bool bWarnIfMissing) override;

	UPROPERTY(EditAnywhere, Category = Appearance, meta = (RequiredAssetDataTags = "RowStructure=CustomRichImageRow"))
	class UDataTable* CustomImageSet;

private:
	FCustomRichImageRow* FindCustomImageRow(FName TagOrId, bool bWarnIfMissing);
};
