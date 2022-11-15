// Fropple Studios Copyright


#include "Actors/CutsceneCameraActor.h"
#include "Subsystems/CameraManagerSubsystem.h"



void ACutsceneCameraActor::BeginPlay()
{
	Super::BeginPlay();
	if (!UniqueCameraID.IsEmpty()) {
		UCameraManagerSubsystem* CameraManager = GetWorld()->GetSubsystem<UCameraManagerSubsystem>();
		if (CameraManager) {
			CameraManager->AddCameraActor(UniqueCameraID, this);
		}
	}
}

void ACutsceneCameraActor::BecomeViewTarget(APlayerController* PC)
{
	Super::BecomeViewTarget(PC);

}
