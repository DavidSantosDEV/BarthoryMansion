// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Gramophone.h"
#include "GameFramework/Actor.h"
#include "MyInterfaces/CommonInputInterface.h"
#include "MyInterfaces/InteractInterface.h"
#include "DrawerActor.generated.h"


class UStaticMeshComponent;
class UDialogueComponent;
class UTimelineComponent;


UENUM()
enum EDrawerStatus
{
	Closed	UMETA(DisplayName = "Closed"),
	Closing	UMETA(DisplayName = "Closing"),
	Opening	UMETA(DisplayName = "Opening"),
	Open	UMETA(DisplayName = "Open"),
};


UCLASS()
class BARTHORYMANSION_API ADrawerActor : public AActor, public IInteractInterface, public ICommonInputInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrawerActor();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drawer")
	UStaticMeshComponent* MainDrawerMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drawer")
	UStaticMeshComponent* LeftDrawer;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drawer")
	UStaticMeshComponent* RightDrawer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drawer")
	UDialogueComponent* DialogueComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Drawer")
	UTimelineComponent* MyTimeline;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drawer")
	UCurveFloat* FloatCurve;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	UDataTable* GiveDiscDialogue;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Dialogue", meta = (RequiredAssetDataTags = "RowStructure=Dialogue"))
	UDataTable* NoItemDiscDialogue;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Drawer")
	FMusicAndNames MusicItem;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drawer")
	float OpenAngle = 90.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Drawer")
	float OpeningTime = 1.f;

	TEnumAsByte<EDrawerStatus> StatusOf = EDrawerStatus::Closed;
	bool bHasGivenDisc = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	void OpenDrawer();
	void CloseDrawer();

	UFUNCTION()
	void OnTimelimeTick(float deltaValue);
	UFUNCTION()
	void OnTimelineFinished();

	UFUNCTION()
	void OnDialogueFinished();

	UFUNCTION()
	void OnGameLoaded(class UMansionSaveGame* SaveData);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	bool GetIsInteractible_Implementation();
	EInteractibleType GetInteractionType_Implementation();
	void Interact_Implementation(AMyPlayerController* PlayerInstigating);

	void BottomFaceButton_Implementation();

	void SetDrawerStatus(EDrawerStatus newStatus, bool bNewHasDisc);

	
	TEnumAsByte<EDrawerStatus> GetDrawerStatus();
	bool GetHasGivenDisc() { return bHasGivenDisc; };
};
