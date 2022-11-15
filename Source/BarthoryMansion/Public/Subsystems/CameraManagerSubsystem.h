// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraManagerSubsystem.generated.h"

class ACutsceneCameraActor;

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UCameraManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	AActor* ActiveCamera;
	TMap<FString, ACutsceneCameraActor*> CameraList;
public:
	
	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	void AddCameraActor(FString CameraID, ACutsceneCameraActor* theCameraActor);

	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	ACutsceneCameraActor* GetCameraWithID(FString CameraID);
	
	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	AActor* GetPlayerCurrentCamera();
	
	UFUNCTION(BlueprintCallable, Category = "CameraSystem")
	void SetPlayerCamera(AActor* newActiveCamera);
};
