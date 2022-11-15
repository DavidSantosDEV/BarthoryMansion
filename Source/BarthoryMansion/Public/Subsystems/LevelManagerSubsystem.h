// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "LevelManagerSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API ULevelManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	TArray<FName> LoadedLevels;
public:
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void LoadLevel(FName LevelToLoad);
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void LoadMultiple(TArray<FName> Levels);
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void UnloadLevel(FName LevelToUnload);
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void UnloadMultiple(TArray<FName> Levels);
	UFUNCTION(BlueprintCallable, Category = "Level Manager")
	void UnloadAllLoadThis(TArray<FName> LevelsToLoad);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Level Manager")
	FORCEINLINE	TArray<FName> GetLoadedLevels() { return LoadedLevels; };
};
