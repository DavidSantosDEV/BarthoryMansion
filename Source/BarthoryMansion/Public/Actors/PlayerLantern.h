// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerLantern.generated.h"

UCLASS()
class BARTHORYMANSION_API APlayerLantern : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Lantern", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MyRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Lantern", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* MyMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Lantern", meta = (AllowPrivateAccess = "true"))
	class UPointLightComponent* MyLight;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Lantern", meta = (AllowPrivateAccess = "true"))
	USoundBase* SoundOn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Lantern", meta = (AllowPrivateAccess = "true"))
	USoundBase* SoundOff;
protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Lantern")
	void OnLightToggled(const bool bValue);
public:	
	// Sets default values for this actor's properties
	APlayerLantern();
public:
	UFUNCTION(BlueprintCallable, Category = "Player Lantern")
	void ToggleLight(bool bNewValue);


};
