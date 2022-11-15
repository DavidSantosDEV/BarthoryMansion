// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractibleActorBase.h"
#include "MyInterfaces/InteractInterface.h"
#include "ItemUsingInteractible.generated.h"

UCLASS()
class BARTHORYMANSION_API AItemUsingInteractible :public AInteractibleActorBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Item, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* myMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraView;
	AActor* PreviousView;

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Interactive")
	bool bRemoveItem=false;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Interactive")
	TSubclassOf<class UItem> RequiredItemClass;

public:	
	// Sets default values for this actor's properties
	AItemUsingInteractible();

	void Interact_Implementation(AMyPlayerController* PlayerInstigating);

	UFUNCTION(BlueprintCallable)
	void ItemUsed();

	UFUNCTION(BlueprintNativeEvent, Category = "Interaction")
	void OnItemUsed();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void ResetPlayerView();

	void OnItemUsed_Implementation();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Interaction")
	FORCEINLINE	AActor* GetPreviousView() { return PreviousView; };
};
