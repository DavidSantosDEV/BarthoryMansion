// Fropple Studios Copyright


#include "Interaction/InteractibleActorBase.h"
#include "Subsystems/SavingSubsystem.h"
#include "SavingSystem/MansionSaveGame.h"

// Sets default values
AInteractibleActorBase::AInteractibleActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bIsInteractible = true;
	bNeedsHold = false;
	fHoldTime = 0.f;
}

void AInteractibleActorBase::OnGameLoadedSave(UMansionSaveGame* Save)
{
	if (Save->InteractiblesData.Contains(GetName())) {
		FInteractibleData* data = Save->InteractiblesData.Find(GetName());
		bShouldExist = data->bShouldExist;
		bIsInteractible = data->bInteractible;
		bIsCompleted = data->bIsComplete;

		SetIsComplete(bIsCompleted);
		SetActorHiddenInGame(!bShouldExist);
		SetActorEnableCollision(bShouldExist);
	}
}

void AInteractibleActorBase::BeginPlay()
{
	Super::BeginPlay();

	USavingSubsystem* savingSystem = GetGameInstance()->GetSubsystem<USavingSubsystem>();
	bool tempComplete=bIsCompleted;
	if (savingSystem) {
		/*UMansionSaveGame* Data = savingSystem->GetCurrentLoadedSaveData();
		if (IsValid(Data)) {

			
			OnGameLoadedSave(Data);
		}*/
		savingSystem->GetInteractibleState(this, bShouldExist, bIsInteractible, tempComplete);
		
		savingSystem->OnGameLoadedSave.AddUniqueDynamic(this, &AInteractibleActorBase::OnGameLoadedSave);
	}
	SetShouldExist(bShouldExist);
	SetIsComplete(tempComplete);
}

void AInteractibleActorBase::EndPlay(const EEndPlayReason::Type EndPlayReaso)
{
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Unloaded %s"), *GetName()));
	}
	//USavingSubsystem* savingSystem = GetGameInstance()->GetSubsystem<USavingSubsystem>();
	//if (savingSystem) {
		//savingSystem->AddInteractibleTempData(this);
	//}
}

bool AInteractibleActorBase::GetIsInteractible_Implementation()
{
	return bIsInteractible;
}

void AInteractibleActorBase::SetIsInteractible_Implementation(const bool bToggle)
{
	bIsInteractible = bToggle;
	OnSetInteractible(bIsInteractible);
}

void AInteractibleActorBase::SetPreviousData(const bool ShouldExist, const bool IsInteractible)
{
	bIsInteractible = IsInteractible;
	bShouldExist = ShouldExist;
}

void AInteractibleActorBase::SetIsComplete(bool bComplete)
{
	bIsCompleted = bComplete;
	OnSetIsComplete(bIsCompleted);
}

void AInteractibleActorBase::SetShouldExist(bool newShouldExist)
{
	bShouldExist = newShouldExist;
	SetActorHiddenInGame(!bShouldExist);
	SetActorEnableCollision(bShouldExist);
	OnSetShouldExist(newShouldExist);
}
