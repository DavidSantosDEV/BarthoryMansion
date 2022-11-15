// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemInspectionStudio.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FRequestToStopInspection);

class AInspectableItem;
class UPointLightComponent;
class UInspectionCollisionComponent;
class UWidgetComponent;
UCLASS()
class BARTHORYMANSION_API AItemInspectionStudio : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStudio, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ActorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStudio, meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* MyParticles;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStudio, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraView;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = ItemStudio, meta = (AllowPrivateAccess = "true"))
	class UBillboardComponent* ItemLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ItemStudio, meta = (AllowPrivateAccess = "true"))
	UPointLightComponent* myLight;




	AActor* PreviousViewTarget;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInspection")
	TSubclassOf<class UWidgetComponent> WidgetCompClass;

	UPROPERTY(EditDefaultsOnly, Category = "ItemInspection")
	float fSpeedRotation = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "ItemInspection")
	FRotator ItemInitialRotation;

	FTimerHandle thTraceLine;

	UPROPERTY(EditDefaultsOnly, Category = "ItemInspection")
	TEnumAsByte<ECollisionChannel> colTraceChannel;

	UPROPERTY(EditDefaultsOnly, Category = "ItemInspection")
	TEnumAsByte<ECollisionChannel> ObjectChannel;

	UPROPERTY(EditDefaultsOnly, Category = "ItemInspection")
	float fTraceTimePause = 0.2f;

	UPROPERTY(BlueprintReadOnly, Category = "ItemInspection")
	AInspectableItem* currentItem;
	
	UPROPERTY(BlueprintReadOnly, Category = "ItemInspection")
	UInspectionCollisionComponent* currentInspectionCollided;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemInspection")
	float fLineDistance;
public:	
	// Sets default values for this actor's properties
	AItemInspectionStudio();

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	void TraceForInteractions();
public:	
	UFUNCTION(BlueprintCallable, Category = "Item Inspection")
	void SetCurrentItem(TSubclassOf<AInspectableItem> newItem);
	UFUNCTION(BlueprintCallable, Category = "Item Inspection")
	void StartTracingForItem();
	UFUNCTION(BlueprintCallable, Category = "Item Inspection")
	void StopTracingForItem();

	UFUNCTION(BlueprintCallable, Category = "Item Inspection")
	void ResetItemPositionAndRotation();
	UFUNCTION(BlueprintCallable, Category = "Item Inspection")
	void MoveItemRight(float val);
	UFUNCTION(BlueprintCallable, Category = "Item Inspection")
	void MoveItemUp(float val);
	UFUNCTION(BlueprintCallable, Category = "Item Inspection")
	void Inspect();

	FRequestToStopInspection OnInspectionToStop;
};
