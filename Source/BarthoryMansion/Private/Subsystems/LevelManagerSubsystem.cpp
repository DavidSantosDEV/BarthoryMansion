// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/LevelManagerSubsystem.h"
#include "Engine/LevelStreaming.h"
#include "Kismet/GameplayStatics.h"


void ULevelManagerSubsystem::LoadLevel(FName LevelToLoad)
{
	if (LevelToLoad == "") return;
	if (LoadedLevels.Contains(LevelToLoad)) {
		return;
	}
	FLatentActionInfo LatentInfo;
	UGameplayStatics::LoadStreamLevel(this, LevelToLoad, true, true, LatentInfo);
	LoadedLevels.AddUnique(LevelToLoad);
}

void ULevelManagerSubsystem::LoadMultiple(TArray<FName> Levels)
{
	for (int i = 0; i < Levels.Num(); ++i) {
		LoadLevel(Levels[i]);
	}
}

void ULevelManagerSubsystem::UnloadLevel(FName LevelToUnload)
{
	if (LevelToUnload == "") return;
	if (LoadedLevels.Contains(LevelToUnload)) {
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, LevelToUnload, LatentInfo, true);
		LoadedLevels.Remove(LevelToUnload);
	}
}

void ULevelManagerSubsystem::UnloadMultiple(TArray<FName> Levels)
{
	for (int i = 0; i < Levels.Num();++i) {
		UnloadLevel(Levels[i]);
	}
}

void ULevelManagerSubsystem::UnloadAllLoadThis(TArray<FName> LevelsToLoad)
{
	for (int i = 0; i < LoadedLevels.Num(); ++i) {
		if (!LevelsToLoad.Contains(LoadedLevels[i])) {
			UnloadLevel(LoadedLevels[i]);
		}
		
	}
	if (LevelsToLoad.Num()>0) {
		for (int i = 0; i < LevelsToLoad.Num(); ++i) {
			LoadLevel(LevelsToLoad[i]);
		}
	}

}

	/*ULevelStreaming* level = UGameplayStatics::GetStreamingLevel(GetWorld(), name);
	if (level) {
		ULevel* sLevel = level->GetLoadedLevel();
		if (sLevel) {
			for (AActor* iActors : sLevel->Actors) {
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, *iActors->GetName());
				}
			}
		}
	}*/

