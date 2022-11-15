// Fropple Studios Copyright


#include "DialogueFunctionChangeCamera.h"
#include "Subsystems/CameraManagerSubsystem.h"
#include "Actors/CutSceneCameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

void UDialogueFunctionChangeCamera::Function(UDialogueComponent* CallingComponent)
{
	UWorld* const World = GetWorld();
	UCameraManagerSubsystem* CameraManager = World->GetSubsystem<UCameraManagerSubsystem>();
	if (World && CameraManager) {
		APlayerController* PlayerC = UGameplayStatics::GetPlayerController(World, 0);
		if (PlayerC) {
			AActor* SelectedCamera;
			if (CameraID == "PlayerCamera") {
				SelectedCamera = CameraManager->GetPlayerCurrentCamera();
			}
			else {
				SelectedCamera = CameraManager->GetCameraWithID(CameraID);
			}
			if (SelectedCamera) {
				PlayerC->SetViewTarget(SelectedCamera);
			}
		}
	}
	
}
