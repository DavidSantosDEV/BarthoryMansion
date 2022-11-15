// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterfaces/InteractInterface.h"
#include "MyInterfaces/CommonInputInterface.h"
#include "Gramophone.generated.h"

USTRUCT(BlueprintType)
struct FMusicAndNames
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Student")
	FString NameOfMusic;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Student")
	class UFMODEvent* MusicEvent;

	FMusicAndNames(FString Name, UFMODEvent* Music) {
		NameOfMusic = Name;
		MusicEvent = Music;
	}
	FMusicAndNames() {
		MusicEvent = nullptr;
		NameOfMusic = "";
	}
};

UCLASS()
class BARTHORYMANSION_API AGramophone : public AActor, public IInteractInterface, public ICommonInputInterface
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	class UDialogueComponent* DialogueComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	class UDataTable* DTdialogue;

	UPROPERTY(BlueprintReadOnly)
	TArray<class UDiscItem*> CastedItems;

	UPROPERTY(BlueprintReadOnly)
	TArray<FMusicAndNames> MusicList;
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void DialogueStopped();

	UFUNCTION(BlueprintImplementableEvent, Category = "Gram")
	void ShowGramUI();

public:	
	// Sets default values for this actor's properties
	AGramophone();

	void Interact_Implementation(class AMyPlayerController* PlayerInstigating);

	void BottomFaceButton_Implementation();
};
