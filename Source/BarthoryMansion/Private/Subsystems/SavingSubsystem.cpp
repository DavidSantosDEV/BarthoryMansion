// Fropple Studios Copyright


#include "Subsystems/SavingSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SavingSystem/MansionSaveGame.h"
#include "Subsystems/CameraManagerSubsystem.h"
#include "Characters/PlayerCharacter.h"
#include "Characters/NPCCharacter.h"
#include "Actors/DrawerActor.h"
#include "MyComponents/DialogueComponent.h"
#include "Inventory/InventoryComponent.h"
#include "Inventory/Item.h"
#include "Actors/SplineFollowCamera.h"
#include "Interaction/InteractibleActorBase.h"
#include "Myinterfaces/InteractInterface.h"
#include "MansionGameMode.h"
#include "Engine/DataTable.h"
#include "Subsystems/LevelManagerSubsystem.h"

void USavingSubsystem::LoadInGame()
{
	if (!CurrentLoadedData) {
		CurrentLoadedData = Cast<UMansionSaveGame>(UGameplayStatics::LoadGameFromSlot(UNIQUE_SAVE_SLOT, 0));
		if (!CurrentLoadedData) {
			return;
		}
	}
	UWorld* const World = GetWorld();
	if (!World) return;

	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (!Player)return;
	FString const NameOfLevel = UGameplayStatics::GetCurrentLevelName(World, true);
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Purple, NameOfLevel);
	}
	if(CurrentLoadedData->LevelData.Contains(NameOfLevel)) {
		FLevelData LevelData = *CurrentLoadedData->LevelData.Find(NameOfLevel);

		Player->SetActorTransform(LevelData.PreviousPlayerTransform);

		TArray<AActor*> outActors;
		UGameplayStatics::GetAllActorsOfClass(World, ASplineFollowCamera::StaticClass(), outActors);

		for (int i = 0; i < outActors.Num();++i) {
			if (outActors[i]->GetName() == LevelData.CurrentActiveCamera) {
				ASplineFollowCamera* Cam = Cast<ASplineFollowCamera>(outActors[i]);
				if (Cam) {
					Cam->ToggleCamera(true);
				}
				break;
			}
		}
	}	
	AMansionGameMode* Gamemode = Cast<AMansionGameMode>(World->GetAuthGameMode());
	if (Gamemode) {
		Gamemode->ChangeMusic(CurrentLoadedData->MusicEvent);
	}
	Player->GetInventoryComponent()->SetMusicItems(CurrentLoadedData->MusicList);

	Player->SetCanUseLantern(CurrentLoadedData->bCanUseLantern);
	Player->GetInventoryComponent()->SetItems(CurrentLoadedData->InventoryItems);
	OnGameLoadedSave.Broadcast(CurrentLoadedData);
		
}


void USavingSubsystem::MakeMeCrash()
{
	APlayerController* Ptr = nullptr;
	FHitResult outhit;
	Ptr->SetActorEnableCollision(true);
}

void USavingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	if (DoesSaveDataExist()) {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("There is a save"));
		}
	}
	else {
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("No Save Data"));
		}
	}
}

TMap<FString, FInteractibleData> USavingSubsystem::GetAllInteractiblesData()
{
	TArray<AActor*> outActors;
	UWorld* const World = GetWorld();
	if (World) {
		//TMap<FString, FInteractibleData> InteractibleTempData;
		UGameplayStatics::GetAllActorsOfClass(World, AInteractibleActorBase::StaticClass(), outActors);
		for (int i = 0; i < outActors.Num(); ++i)
		{
			if (outActors[i]) {
				AInteractibleActorBase* const ActorInt = Cast<AInteractibleActorBase>(outActors[i]);
				if (ActorInt)
				{
					FString const name = ActorInt->GetName();
					FInteractibleData foundData = TempInteractiblesData.FindOrAdd(name);
					foundData.bInteractible = IInteractInterface::Execute_GetIsInteractible(ActorInt);
					foundData.bShouldExist = ActorInt->GetShouldExist();
					foundData.bIsComplete = ActorInt->GetIsComplete();
					TempInteractiblesData.Emplace(name, foundData);

					/*FInteractibleData data;// = FInteractibleData(ActorInt->GetShouldExist(), IInteractInterface::Execute_GetIsInteractible(ActorInt));
					data.bInteractible = IInteractInterface::Execute_GetIsInteractible(ActorInt);
					data.bShouldExist = ActorInt->GetShouldExist();
					InteractibleTempData.Add(name, data);*/
				}
			}

		}
	}

	return TempInteractiblesData;
}

TMap<FString, FLevelData> USavingSubsystem::GetAllLevelsData()
{
	UWorld* const World = GetWorld();
	
	if (World) {

		AActor* PlayerP = UGameplayStatics::GetPlayerPawn(World, 0);

		FString const WorldName = UGameplayStatics::GetCurrentLevelName(World, true);
		FLevelData LevelData = TempLevelData.FindOrAdd(WorldName);
		
		UCameraManagerSubsystem* CamMan = World->GetSubsystem<UCameraManagerSubsystem>();
		if (CamMan) {
			AActor* CameraActor = CamMan->GetPlayerCurrentCamera();
			if (CameraActor) {
				LevelData.CurrentActiveCamera = CameraActor->GetName();
			}
		}	
		if (PlayerP) {
			LevelData.PreviousPlayerTransform = PlayerP->GetActorTransform();
		}

		TempLevelData.Emplace(WorldName, LevelData);
	}

	return TempLevelData;
}

TMap<FString, FDrawerData> USavingSubsystem::GetAllDrawersData()
{
	TArray<AActor*> outActors;
	//TMap<FString, FInteractibleData> InteractibleTempData;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADrawerActor::StaticClass(), outActors);
	for (int i = 0; i < outActors.Num(); ++i)
	{
		if (outActors[i]) {
			ADrawerActor* const ActorInt = Cast<ADrawerActor>(outActors[i]);
			if (ActorInt)
			{
				FString const name = ActorInt->GetName();

				FDrawerData DrawerD = TempDrawerData.FindOrAdd(name);
				DrawerD.bHasGivenMusic = ActorInt->GetHasGivenDisc();
				DrawerD.DrawerStatus = ActorInt->GetDrawerStatus();

				TempDrawerData.Emplace(name, DrawerD);
			}
		}

	}
	return TempDrawerData;
}

TMap<FString, FNPCData> USavingSubsystem::GetAllNPCData()
{
	TArray<AActor*> outActors;
	//TMap<FString, FInteractibleData> InteractibleTempData;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ANPCCharacter::StaticClass(), outActors);
	for (int i = 0; i < outActors.Num(); ++i)
	{
		if (outActors[i]) {
			ANPCCharacter* const ActorInt = Cast<ANPCCharacter>(outActors[i]);
			if (ActorInt)
			{
				FString const name = ActorInt->GetName();

				FNPCData foundData = TempNPCData.FindOrAdd(name);
				foundData.bIsInteractible = IInteractInterface::Execute_GetIsInteractible(ActorInt);
				foundData.bShouldExist = ActorInt->GetShouldExist();
				foundData.bIsRepeatingDialogue = ActorInt->GetDialogueComponent()->GetIsRepeatingDialogue();
				foundData.CurrentDialogueData = ActorInt->GetDialogueComponent()->GetCurrentDialogue();
				foundData.CachedDialogue = ActorInt->GetDialogueComponent()->GetCachedDialogue();
				
				TempNPCData.Emplace(name, foundData);

				/*FInteractibleData data;// = FInteractibleData(ActorInt->GetShouldExist(), IInteractInterface::Execute_GetIsInteractible(ActorInt));
				data.bInteractible = IInteractInterface::Execute_GetIsInteractible(ActorInt);
				data.bShouldExist = ActorInt->GetShouldExist();
				InteractibleTempData.Add(name, data);*/
			}
		}

	}
	return TempNPCData;
}

void USavingSubsystem::LoadPlayerData()
{
	UWorld* const World = GetWorld();

	if (World) {
		APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
		UMansionSaveGame* Slot = Cast<UMansionSaveGame>(UGameplayStatics::LoadGameFromSlot(UNIQUE_SAVE_SLOT, 0));
		if (Slot) {
			if (Player) {		
				FString NameOfLevel = UGameplayStatics::GetCurrentLevelName(World, true);
				if (Slot->LevelData.Contains(NameOfLevel)) {
					FLevelData LevelData = *Slot->LevelData.Find(NameOfLevel);
					TempLevelData.Emplace(NameOfLevel, LevelData);


					Player->SetActorTransform(LevelData.PreviousPlayerTransform);

					TArray<AActor*> OutActor;
					UGameplayStatics::GetAllActorsOfClass(World, ASplineFollowCamera::StaticClass(), OutActor);
					for (AActor* InA : OutActor) {
						if (InA && InA->GetName() == LevelData.CurrentActiveCamera) {
							ASplineFollowCamera* Cam = Cast<ASplineFollowCamera>(InA);
							if (Cam) {
								Cam->ToggleCamera(true);
							}
							break;
						}
					}
				}
				Player->SetCanUseLantern(Slot->bCanUseLantern);
				UInventoryComponent* Inventory = Player->GetInventoryComponent();
				if (Inventory) {
					Inventory->SetItems(Slot->InventoryItems);
					Inventory->SetMusicItems(Slot->MusicList);
				}
				//Player->GetInventoryComponent()->SetItems(CurrentLoadedData->InventoryItems);
			}
		}
		
	}	
}

void USavingSubsystem::GetInteractibleState(AInteractibleActorBase* InteractActor, bool& outBShouldExist, bool& outBIsInteractible, bool& outbIsComplete)
{
	if (!InteractActor) return;
	if (TempInteractiblesData.Num() <= 0)return;
	const FString name = InteractActor->GetName();

	if (TempInteractiblesData.Contains(name)) {
		FInteractibleData data = TempInteractiblesData.FindOrAdd(name);
		outBShouldExist = data.bShouldExist;
		outBIsInteractible = data.bInteractible;
		outbIsComplete = data.bIsComplete;
	}
}

void USavingSubsystem::GetDrawerState(ADrawerActor* Drawer, bool& outGaveMusic, TEnumAsByte<EDrawerStatus>& outDrawerStat)
{
	if (!Drawer || TempDrawerData.Num() <= 0)return;
	const FString name = Drawer->GetName();
	if (TempDrawerData.Contains(name)) {
		FDrawerData data = *TempDrawerData.Find(name);
		outDrawerStat = data.DrawerStatus;
		outGaveMusic = data.bHasGivenMusic;
	}
}

void USavingSubsystem::GetNPCState(ANPCCharacter* NPC, bool& outBShouldExist, bool& outBIsInteractible)
{
	if (!NPC) return;
	if (TempNPCData.Num() <= 0)return;
	const FString name = NPC->GetName();

	if (TempNPCData.Contains(name)) {
		FNPCData data = TempNPCData.FindOrAdd(name);

		outBShouldExist = data.bShouldExist;
		outBIsInteractible = data.bIsInteractible;
		UDialogueComponent* CompDialogue = NPC->GetDialogueComponent();
		if (CompDialogue) {
			CompDialogue->SetCachedDialogue(data.CachedDialogue);
			CompDialogue->ChangeDialogueData(data.CurrentDialogueData);
			CompDialogue->SetWasRepeatingDialogue(data.bIsRepeatingDialogue);
		}
	}
}

bool USavingSubsystem::CreateSaveGame()
{
	return false;
}

void USavingSubsystem::AddInteractibleTempData(AInteractibleActorBase* InteractActor)
{
	const FString name = InteractActor->GetName();
	if (!TempInteractiblesData.Contains(name)) {
		TempInteractiblesData.Add(name);
	}
	FInteractibleData* data;
	data = TempInteractiblesData.Find(name);
	if (data) {
		data->bInteractible = IInteractInterface::Execute_GetIsInteractible(InteractActor);
		data->bShouldExist = InteractActor->GetShouldExist();
		data->bIsComplete = InteractActor->GetIsComplete();
	}
}

void USavingSubsystem::AddNPCTempData(ANPCCharacter* NPCActor)
{
	const FString name = NPCActor->GetName();
	if (!TempNPCData.Contains(name)) {
		TempNPCData.Add(name);
	}
	FNPCData* data;
	data = TempNPCData.Find(name);
	if (data) {
		data->bIsInteractible = IInteractInterface::Execute_GetIsInteractible(NPCActor);
		data->bShouldExist = NPCActor->GetShouldExist();
		data->bIsRepeatingDialogue = NPCActor->GetDialogueComponent()->GetIsRepeatingDialogue();
		data->CurrentDialogueData = NPCActor->GetDialogueComponent()->GetCurrentDialogue();
		data->CachedDialogue = NPCActor->GetDialogueComponent()->GetCachedDialogue();
	}
}

bool USavingSubsystem::SaveGameProgress()
{
	UWorld* const World = GetWorld();
	if (!World)return false;
	UMansionSaveGame* newSave = Cast<UMansionSaveGame>(UGameplayStatics::CreateSaveGameObject(UMansionSaveGame::StaticClass()));
	APlayerCharacter* const Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(World, 0));
	if (Player)
	{
		ASplineFollowCamera* const cam = Cast<ASplineFollowCamera>(Player->GetController()->GetViewTarget());
		if (cam)
		{
			newSave->CreationTime = FDateTime::Now();

			newSave->LevelData = GetAllLevelsData();

			newSave->bCanUseLantern = Player->GetCanUseLantern();

			TArray<UItem*> AllItems = Player->GetInventoryComponent()->GetItems();
			TArray<FMyItemData> ItemData;
			for (int i = 0; i < AllItems.Num(); ++i) {
				FMyItemData myData;
				myData.Item = AllItems[i];
				myData.ItemClass = AllItems[i]->GetClass();
				ItemData.Add(myData);
			}
			newSave->InventoryItems = ItemData;
			newSave->MusicList = Player->GetInventoryComponent()->GetMusicItems();
			newSave->InteractiblesData = GetAllInteractiblesData();
			newSave->DrawersData = GetAllDrawersData();
			newSave->NPCProgress = GetAllNPCData();
			newSave->SavedBoolValues = TempBoolValues;
			//AMansionGameMode* Gamemode = Cast<AMansionGameMode>(GetWorld()->GetAuthGameMode());
			//if (Gamemode) {
			//	newSave->MusicEvent = Gamemode->GetCurrentMusic();
			//}
			if (!IsValid(newSave)) {
				return false;
			}
			bool saved = UGameplayStatics::SaveGameToSlot(newSave, UNIQUE_SAVE_SLOT, 0);
			if (saved) {
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Saved Game"));
				}
				CurrentLoadedData = newSave;
				return true;
			}
			else {
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Failed to Saved Game"));
				}
				return false;
			}
		}
	}

	return false;
}

bool USavingSubsystem::LoadGameToMemory(bool bLoadStuff)
{
	if (!DoesSaveDataExist()) {
		return false;
	}
	UMansionSaveGame* Slot = Cast<UMansionSaveGame>(UGameplayStatics::LoadGameFromSlot(UNIQUE_SAVE_SLOT, 0));
	if (Slot) 
	{
		CurrentLoadedData = Slot;
		TempInteractiblesData = Slot->InteractiblesData;
		TempDrawerData = Slot->DrawersData;
		TempNPCData = Slot->NPCProgress;
		TempLevelData = Slot->LevelData;
		TempBoolValues = Slot->SavedBoolValues;
		CurrentLevelName = Slot->CurrentLevel;
		if (bLoadStuff) {
			LoadInGame();
			LoadPlayerData();
		}
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Loaded Game"));
		}
		return true;
	}
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("Could not Load Game"));
	}
	return false;
}

bool USavingSubsystem::DoesSaveDataExist()
{
	return UGameplayStatics::DoesSaveGameExist(UNIQUE_SAVE_SLOT, 0);
}

bool USavingSubsystem::ClearExistingSaveData()
{
	CurrentLoadedData = nullptr;
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("Deleting Save Data")));
	}
	return UGameplayStatics::DeleteGameInSlot(UNIQUE_SAVE_SLOT, 0);
}

void USavingSubsystem::ClearTempData()
{
	CurrentLoadedData = nullptr;
	TempInteractiblesData.Empty();
	TempDrawerData.Empty();
	TempNPCData.Empty();
	TempLevelData.Empty();
	TempBoolValues.Empty();
}

UMansionSaveGame* USavingSubsystem::GetCurrentTempData()
{
	return CurrentLoadedData;
}

bool USavingSubsystem::IsDataLoaded()
{
	return CurrentLoadedData != nullptr;
}

void USavingSubsystem::SaveGameFinished()
{
	UMansionSaveGame* newSave = Cast<UMansionSaveGame>(UGameplayStatics::CreateSaveGameObject(UMansionSaveGame::StaticClass()));
	if (newSave) {
		newSave->bGameIsFinished = true;
		UGameplayStatics::SaveGameToSlot(newSave, UNIQUE_SAVE_SLOT, 0);
	}
}

bool USavingSubsystem::IsSlotGameFinished()
{
	if(UGameplayStatics::DoesSaveGameExist(UNIQUE_SAVE_SLOT, 0)) {
		UMansionSaveGame* Slot = Cast<UMansionSaveGame>(UGameplayStatics::LoadGameFromSlot(UNIQUE_SAVE_SLOT, 0));
		if (Slot) {
			return Slot->bGameIsFinished;
		}
	}

	return false;
}

void USavingSubsystem::AddBoolValue(FString Name, bool bVal)
{
	if (TempBoolValues.Contains(Name)) {
		TempBoolValues.Emplace(Name, bVal);
	}
	else {
		TempBoolValues.Add(Name, bVal);
	}
}

bool USavingSubsystem::GetValueAsBool(FString KeyName, bool& outVal)
{
	if (TempBoolValues.Find(KeyName)) {
		outVal = TempBoolValues.FindRef(KeyName);
		return true;
	}
	return false;
}

void USavingSubsystem::RemoveValueAsBool(FString KeyName)
{
	if (TempBoolValues.Contains(KeyName)) {
		TempBoolValues.Remove(KeyName);
	}
}

void USavingSubsystem::AddFloatValue(FString Name, float bVal)
{
	if (TempFloatValues.Contains(Name)) {
		TempFloatValues.Emplace(Name, bVal);
	}
	else {
		TempFloatValues.Add(Name, bVal);
	}
}

bool USavingSubsystem::GetValueAsFloat(FString KeyName, float& outVal)
{
	if (TempFloatValues.Find(KeyName)) {
		outVal = TempFloatValues.FindRef(KeyName);
		return true;
	}
	return false;
}
