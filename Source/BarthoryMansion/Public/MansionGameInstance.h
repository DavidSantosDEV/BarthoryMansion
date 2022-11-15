// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MansionGameInstance.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPauseToggle, bool, bIsPaused);
DECLARE_DYNAMIC_DELEGATE(FMyDel);

/**
 * 
 */
UCLASS(Config = Game)
class BARTHORYMANSION_API UMansionGameInstance : public UGameInstance
{
	GENERATED_BODY()


protected:
#pragma region Configurable
	UPROPERTY(Config, EditAnywhere)
	bool bShowTutorials;
	UPROPERTY(Config, EditAnywhere)
	float fVolumeMain;
	UPROPERTY(Config, EditAnywhere)
	float fVolumeSFX;
	UPROPERTY(Config, EditAnywhere)
	float fVolumeMusic;
	UPROPERTY(Config, EditAnywhere)
	int IconIndex;
	UPROPERTY(Config, EditAnywhere)
	bool bShouldLoadGame = false;

#pragma endregion

	virtual void Init() override;

public:


#pragma region PausingGame
	//FOnPauseToggle OnGamePauseToggle;

	UFUNCTION(BlueprintCallable, Category="Pausing")
	void TogglePauseGame();


	UFUNCTION(BlueprintCallable, Category = "Loading")
	void SetShouldLoad(bool bShould);

	UFUNCTION(BlueprintPure, BlueprintCallable)
	bool GetShouldLoad() { return bShouldLoadGame; }

#pragma endregion
	UFUNCTION(BlueprintCallable, Category = "GamepadIcons")
	void SetGamepadIcons(int index);

	UFUNCTION(BlueprintCallable, Category = "Config")
	void SaveAllConfigData();
	UFUNCTION(BlueprintCallable, Category = "Config")
	void AlterVolumeMain(float newVal);
	UFUNCTION(BlueprintCallable, Category = "Config")
	void AlterVolumeMusic(float newVal);
	UFUNCTION(BlueprintCallable, Category = "Config")
	void AlterVolumeEnv(float newVal);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Config")
	float GetMainVolume() const { return fVolumeMain; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Config")
	float GetMusicVolume() const { return fVolumeMusic; };
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Config")
	float GetSFXVolume() const { return fVolumeSFX; };
};
