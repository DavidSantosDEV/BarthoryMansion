// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyInterfaces/CommonInputInterface.h"
#include "MyInterfaces/InteractInterface.h"
#include "InteractibleActorBase.h"
#include "InteractiveDialogue.generated.h"

UCLASS()
class BARTHORYMANSION_API AInteractiveDialogue : public AActor,public IInteractInterface, public ICommonInputInterface
{
	GENERATED_BODY()
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactible Dialogue")
	class UCapsuleComponent* CapsuleCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interactible Dialogue")
	class UDialogueComponent* DialogueComp;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Interactible Dialogue", meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	class UDataTable* DTdialogue;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	

	UFUNCTION()
	void EndDialogue();

	// Sets default values for this actor's properties
	AInteractiveDialogue();

	void BottomFaceButton_Implementation();

	void Interact_Implementation(class AMyPlayerController* PlayerInstigating);
	EInteractibleType GetInteractionType_Implementation();

};
