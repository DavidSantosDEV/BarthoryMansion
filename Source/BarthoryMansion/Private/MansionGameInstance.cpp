// Fill out your copyright notice in the Description page of Project Settings.


#include "MansionGameInstance.h"
#include "Math/IntPoint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameUserSettings.h" 
#include "Subsystems/InputChangeSubsystem.h"
#include "Subsystems\SavingSubsystem.h"

void UMansionGameInstance::Init()
{
    Super::Init();
    USavingSubsystem* savingSystem = GetSubsystem<USavingSubsystem>();
    if (savingSystem) {
        if (GEngine) {
            GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, FString::Printf(TEXT("%s"), savingSystem->DoesSaveDataExist() ? "Has save" : "No Save Data"));
        }
    }
    LoadConfig();
    /*
    TArray<FIntPoint> OutRes;
    UKismetSystemLibrary::GetSupportedFullscreenResolutions(OutRes);
    if (OutRes.Num()>0) {
        for (int i = OutRes.Num() - 1; i > 0; --i) {
            int Calc = OutRes[i].X / 16;
            Calc *= 9;
            if (Calc == OutRes[i].Y) {
                if (!GEngine)break;
                UGameUserSettings* UserSet = GEngine->GameUserSettings;
                if (!UserSet)break;

                GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Orange, FString::Printf(TEXT("%s"), *OutRes[i].ToString()));

                UserSet->SetScreenResolution(OutRes[i]);
                UserSet->ApplyResolutionSettings(false);
                UserSet->ApplySettings(false);
                UserSet->SaveSettings();
                break;
            }
        }
    }*/
    
}

void UMansionGameInstance::TogglePauseGame()
{
    bool bPause = !UGameplayStatics::IsGamePaused(GetWorld());
    UGameplayStatics::SetGamePaused(GetWorld(),bPause);
    //OnGamePauseToggle.Broadcast(bPause);
}

void UMansionGameInstance::SetShouldLoad(bool bShould)
{
    bShouldLoadGame = bShould;
}

void UMansionGameInstance::SetGamepadIcons(int index)
{
    UInputChangeSubsystem* ChangeSubsystem = GEngine->GetEngineSubsystem<UInputChangeSubsystem>();
    ChangeSubsystem->SetIconFamilyIndex(FMath::Clamp(index, 0, 2));
}

void UMansionGameInstance::SaveAllConfigData()
{
    SaveConfig();
}

void UMansionGameInstance::AlterVolumeMain(float newVal)
{
    fVolumeMain = newVal;
    SaveConfig();
}

void UMansionGameInstance::AlterVolumeMusic(float newVal)
{
    fVolumeMusic = newVal;
    SaveConfig();
}

void UMansionGameInstance::AlterVolumeEnv(float newVal)
{
    fVolumeSFX = newVal;
    SaveConfig();
}
