// Fropple Studios Copyright


#include "Subsystems/CameraManagerSubsystem.h"

void UCameraManagerSubsystem::AddCameraActor(FString CameraID, ACutsceneCameraActor* theCameraActor)
{
    if (CameraList.Contains(CameraID)) {
        return;
    }
    CameraList.Add(CameraID, theCameraActor);
}

ACutsceneCameraActor* UCameraManagerSubsystem::GetCameraWithID(FString CameraID)
{
    if (CameraList.Contains(CameraID)) {
        return *CameraList.Find(CameraID);
    }

    return nullptr;
}

AActor* UCameraManagerSubsystem::GetPlayerCurrentCamera()
{
    return ActiveCamera;
}

void UCameraManagerSubsystem::SetPlayerCamera(AActor* newActiveCamera)
{
    if (newActiveCamera) {
        ActiveCamera = newActiveCamera;
    }
}
