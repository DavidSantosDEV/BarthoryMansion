// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractibleActorBase.h"
#include "InspectableItem.generated.h"

UCLASS()
class BARTHORYMANSION_API AInspectableItem : public AInteractibleActorBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = Item)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = Item)
	class UInspectionCollisionComponent* InteractCollision;
	//Custom widget
	UPROPERTY(BlueprintReadOnly)
	class AItemInspectionStudio* MyStudio;
public:	
	// Sets default values for this actor's properties
	AInspectableItem();

	void SetStudio(AItemInspectionStudio* studio);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
