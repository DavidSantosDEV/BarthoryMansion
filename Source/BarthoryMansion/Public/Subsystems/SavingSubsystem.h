// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SavingSystem/MansionSaveGame.h"
#include "SavingSubsystem.generated.h"


class UMansionSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameLoaded, UMansionSaveGame*, Save);
//UGameInstance* GameInstance = ...;
//UMyGameSubsystem* MySubsystem = GameInstance->GetSubsystem<UMyGameSubsystem>();


/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API USavingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	const FString UNIQUE_SAVE_SLOT = "SaveSlot_0";
	UPROPERTY()
	UMansionSaveGame* CurrentLoadedData;
	TMap<FString, FInteractibleData> TempInteractiblesData;
	TMap<FString, FDrawerData> TempDrawerData;
	TMap<FString, FNPCData> TempNPCData;
	TMap<FString, FLevelData> TempLevelData;

	TMap<FString, bool> TempBoolValues;
	TMap<FString, float> TempFloatValues;

	FName CurrentLevelName;
protected:

	TMap<FString, FInteractibleData> GetAllInteractiblesData();

	TMap<FString, FLevelData> GetAllLevelsData();

	TMap<FString, FDrawerData> GetAllDrawersData();

	TMap<FString, FNPCData> GetAllNPCData();

public:

	UFUNCTION(BlueprintCallable)
		void MakeMeCrash();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	UPROPERTY(BlueprintAssignable, Category = "SavingSystem")
	FOnGameLoaded OnGameLoadedSave;

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	void LoadPlayerData();

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	void GetInteractibleState(AInteractibleActorBase* InteractActor, bool& outBShouldExist, bool& outBIsInteractible, bool& outbIsComplete);

	void GetDrawerState(ADrawerActor* Drawer, bool& outGaveMusic, TEnumAsByte<EDrawerStatus>& outDrawerStat);

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	void GetNPCState(class ANPCCharacter* NPC, bool& outBShouldExist, bool& outBIsInteractible);

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	void AddInteractibleTempData(AInteractibleActorBase* InteractActor);

	void AddNPCTempData(ANPCCharacter* NPCActor);

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	bool CreateSaveGame();

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	bool SaveGameProgress();

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	bool LoadGameToMemory(bool bLoadStuff);

	UFUNCTION(BlueprintCallable, Category = "Saving System")
	void LoadInGame();

	UFUNCTION(BlueprintPure,BlueprintCallable, Category = "SavingSystem")
	bool DoesSaveDataExist();

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	bool ClearExistingSaveData();

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	void ClearTempData();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "SavingSystem")
	UMansionSaveGame* GetCurrentLoadedSaveData() { return CurrentLoadedData; };

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "SavingSystem")
	UMansionSaveGame* GetCurrentTempData();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "SavingSystem")
	bool IsDataLoaded();

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	void SaveGameFinished();

	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "SavingSystem")
	bool IsSlotGameFinished();

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	void AddBoolValue(FString Name, bool bVal);
	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	bool GetValueAsBool(FString KeyName, bool& outVal);
	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	void RemoveValueAsBool(FString KeyName);

	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	void AddFloatValue(FString Name, float bVal);
	UFUNCTION(BlueprintCallable, Category = "SavingSystem")
	bool GetValueAsFloat(FString KeyName, float& outVal);

};
