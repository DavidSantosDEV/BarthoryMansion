// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MyComponents/DialogueComponent.h"
#include "Actors/DrawerActor.h"
#include "Actors/Gramophone.h"
#include "MansionSaveGame.generated.h"


USTRUCT(BlueprintType)
struct FLevelData {
	GENERATED_BODY()
public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FString CurrentActiveCamera;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FTransform PreviousPlayerTransform;

	FLevelData() {
		CurrentActiveCamera = "";
		PreviousPlayerTransform = FTransform();
	}
};

USTRUCT(BlueprintType)
struct FNPCData {
	GENERATED_BODY()
public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bIsInteractible;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bShouldExist;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	UDataTable* CurrentDialogueData;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<UDataTable*> CachedDialogue;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bIsRepeatingDialogue;

	FNPCData() {
		bIsInteractible=false;
		
		bShouldExist=false;
		
		CurrentDialogueData = nullptr;
		
		bIsRepeatingDialogue = false;
	}
};

USTRUCT(BlueprintType)
struct FInteractibleData {
	GENERATED_BODY()
public:

	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bInteractible;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bShouldExist;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bIsComplete;

	FInteractibleData() {
		bInteractible = false;
		bShouldExist = false;
		bIsComplete = false;
	}
};

USTRUCT(BlueprintType)
struct FDrawerData {
	GENERATED_BODY()
public:

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TEnumAsByte<EDrawerStatus> DrawerStatus;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	bool bHasGivenMusic;

	FDrawerData() {
		DrawerStatus = EDrawerStatus::Closed;
		bHasGivenMusic = false;
	}
};


USTRUCT(BlueprintType)
struct FMyItemData {
	GENERATED_BODY()
public:

	UPROPERTY(SaveGame,Instanced, BlueprintReadWrite)
	class UItem* Item;
	UPROPERTY(SaveGame, BlueprintReadWrite)
	TSubclassOf<UItem> ItemClass;

	FMyItemData() {
		Item = nullptr;
		ItemClass = nullptr;
	}
};

USTRUCT(BlueprintType)
struct FPLayerData {
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, SaveGame)
	FTransform PlayerTransform;

	UPROPERTY(BlueprintReadWrite, SaveGame) /*Instanced*/
	TArray<FMyItemData> InventoryItems;

	UPROPERTY(BlueprintReadWrite, SaveGame)
	bool bCanUseLantern;

	FPLayerData() {
		PlayerTransform = FTransform();
		bCanUseLantern = false;
	}
};
/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UMansionSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = SaveData)
	bool bGameIsFinished;
    
	UPROPERTY(VisibleAnywhere, Category = SaveData)
	FDateTime CreationTime;

	UPROPERTY(VisibleAnywhere, Category = SaveData)
	FString SaveSlotName;

	UPROPERTY(BlueprintReadWrite, Category = SaveData)
	bool bCanUseLantern;

	UPROPERTY(BlueprintReadWrite, Category = SaveData)
	FName CurrentLevel;

	UPROPERTY(VisibleAnywhere, Category = SaveData) /*Instanced*/
	TArray<FMyItemData> InventoryItems;

	UPROPERTY(VisibleAnywhere, Category = SaveData) /*Instanced*/
	TArray<FMusicAndNames> MusicList;

	UPROPERTY(VisibleAnywhere, Category = SaveData)
	TMap<FString, FNPCData> NPCProgress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData)
	TMap<FString, FInteractibleData> InteractiblesData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData)
	TMap<FString, FDrawerData> DrawersData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData)
	TMap<FString, FLevelData> LevelData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData)
	class UFMODEvent* MusicEvent= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveData)
	TMap<FString, bool> SavedBoolValues;
};
