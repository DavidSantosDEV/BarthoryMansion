// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MyDeveloperSettings.generated.h"

USTRUCT(Blueprintable)
struct FInputToIcon  //No need to specify key
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> IconKeyboard;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> IconXbox;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> IconPS;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UTexture2D> IconNintendo;
};

USTRUCT(Blueprintable)
struct FDialogueSounds  //No need to specify key
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<class USoundBase> CueType;
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundBase> CueFinish;
};

class UDataTable;

UCLASS(config = Game,defaultconfig)
class BARTHORYMANSION_API UMyDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

	UMyDeveloperSettings(const FObjectInitializer& ObjectInitializer);
public:

	//For some reason, functions dont work here, cant have Getters


	//, meta = (RequiredAssetDataTags = "RowStructure=InputToIcon")
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, Config)
	TMap<FName,FInputToIcon> InputIconMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Audio")
	class UFMODEvent* TypeEvent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Audio")
	UFMODEvent* TypeFinishEvent;

	//UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Developer Settings")
	//FORCEINLINE USoundBase* GetTypeSound() { return TypeSound; }

	virtual FName GetContainerName() const override
	{
		return "Project";
	}
	virtual FName GetCategoryName() const override
	{
		return "Game";
	}
	virtual FName GetSectionName() const override
	{
		return "Button Label Settings";
	}
};
