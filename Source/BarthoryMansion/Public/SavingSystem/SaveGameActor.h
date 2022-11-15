// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterfaces/InteractInterface.h"
#include "MyInterfaces/CommonInputInterface.h"
#include "SaveGameActor.generated.h"

UCLASS()
class BARTHORYMANSION_API ASaveGameActor : public AActor, public IInteractInterface, public ICommonInputInterface //Not an important interactible so its named as such
{
	GENERATED_BODY()
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SaveGameActor")
	class UDialogueComponent* DialogueComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveGameActor")
	class UDataTable* DialogueBase;

	UPROPERTY(EditDefaultsOnly, Category = "Save")
	class UFMODEvent* SavingSound;

	UFUNCTION()
	void OnDialogueEnd();
public:	
	// Sets default values for this actor's properties
	ASaveGameActor();
public:	

	virtual void BeginPlay() override;

	void Interact_Implementation(AMyPlayerController * PlayerInstigating);

	UFUNCTION(BlueprintImplementableEvent, Category = "Save")
	void PlayAnimationSave();

	
	void BottomFaceButton_Implementation();
};
