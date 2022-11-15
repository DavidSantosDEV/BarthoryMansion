// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractibleActorBase.h"
#include "MyInterfaces/CommonInputInterface.h"
#include "BoilerPuzzleBase.generated.h"

USTRUCT(BlueprintType)
struct FPieceInfo 
{
	GENERATED_BODY()
public:
	UStaticMeshComponent* DesiredComponent;
	UStaticMeshComponent* GreenLight;
};

USTRUCT(BlueprintType)
struct FLocationAndIndex
{
	GENERATED_BODY()
public:
	UStaticMeshComponent* Comp;
	FVector Location;
	int Index;
};


/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API ABoilerPuzzleBase : public AInteractibleActorBase, public ICommonInputInterface
{
	GENERATED_BODY()



	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MainBoilerMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* PlateMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	class UNiagaraComponent* ParticleBomb;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	class UDialogueComponent* DialogueComp;


	//Screws
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ScrewOne;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ScrewTwo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ScrewThree;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ScrewFour;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CondensorOne;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CondensorTwo;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CondensorThree;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* CondensorFour;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boiler", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraView;

	ABoilerPuzzleBase();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boiler")
	float TimeForExplosion=3;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	class UDataTable* DialogueNoLight;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	UDataTable* DialogueFinish;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	UDataTable* SecondDialogue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boiler")
	class UFMODEvent* SelectionSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boiler")
	UFMODEvent* ExplosionSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boiler")
	UMaterialInterface* MaterialInstance;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boiler")
	FName SelectedParameterName = "Selected";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Boiler")
	TArray<int> OrderArray;

	UPROPERTY(BlueprintReadWrite, Category = "Boiler")
	bool bPlateOpen = false;
	
	TMap<int, UStaticMeshComponent*> ComponentDesiredLocation;

	TArray<FLocationAndIndex> ComponentStartingLocation;
	
	TArray<UStaticMeshComponent*> CondensorList;

	int CurrentIndex = 0;
	int FirstSelectedIndex;
	bool bHasSelectedIndex;
	bool bCanSelect=false;
	bool bDialogueMode = false;
	AActor* PreviousViewTarget;

protected:

	virtual void BeginPlay() override;
	void GenerateConLocation();
	void CheckLocations();
	void MoveIndex(int indexVal);
	void SelectComp(int index,float val);
	void BlowUp();

	void ResetPuzzle();

	UFUNCTION()
	void OnDialogueIsFinished();

	UFUNCTION(BlueprintCallable)
	void ActualBlowUp();

	UFUNCTION()
	void StartSecondDialogue();

	UFUNCTION(BlueprintCallable, Category = "Boiler")
	void InitiatePuzzle();
	UFUNCTION(BlueprintCallable, Category = "Boiler")
	void QuitPuzzle();

	UFUNCTION(BlueprintImplementableEvent, Category = "Boiler")
	void OpenPlate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Boiler")
	void ClosePlate();

	UFUNCTION(BlueprintImplementableEvent, Category = "Boiler")
	void OnPuzzleFinished();

	UFUNCTION(BlueprintImplementableEvent, Category = "Boiler")
	void CreateUI();

	UFUNCTION(BlueprintImplementableEvent, Category = "Boiler")
	void RemoveUI();

public:

	void Interact_Implementation(class AMyPlayerController* PlayerInstigating);

	void BottomFaceButton_Implementation();

	void RightFaceButton_Implementation();

	void DPadRight_Implementation();

	void DPadLeft_Implementation();
	
	//void LeftAnalogRight_Implementation(float Value);
};
