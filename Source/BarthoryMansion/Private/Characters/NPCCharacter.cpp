// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPCCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "MyHUD.h"
#include "MyPlayerController.h"
#include "Engine/DataTable.h" 
#include "MyComponents/DialogueComponent.h"
#include "Camera/CameraComponent.h"
#include "Subsystems/SavingSubsystem.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h" 

ANPCCharacter::ANPCCharacter() 
{
	//InteractionComponent = CreateDefaultSubobject<UInteractComponent>(FName("InteractionComponent"));
	DialogueComponent = CreateDefaultSubobject<UDialogueComponent>(FName("DialogueComponent"));
	CameraView = CreateDefaultSubobject<UCameraComponent>(FName("CameraView"));
	CameraView->SetupAttachment(RootComponent);
}

void ANPCCharacter::BeginPlay()
{
	Super::BeginPlay();
	DialogueComponent->OnStopDialogueEvent.AddUniqueDynamic(this, &ANPCCharacter::StoppedTypingEvent);

	USavingSubsystem* savingSystem = GetGameInstance()->GetSubsystem<USavingSubsystem>();
	if (savingSystem) {
		/*UMansionSaveGame* Data = savingSystem->GetCurrentLoadedSaveData();
		if (IsValid(Data)) {


			OnGameLoadedSave(Data);
		}*/
		savingSystem->GetNPCState(this, bShouldExist, bIsInteractible);

		savingSystem->OnGameLoadedSave.AddUniqueDynamic(this, &ANPCCharacter::OnGameLoaded);
	}
	SetActorHiddenInGame(!bShouldExist);
	SetActorEnableCollision(bShouldExist);
}

void ANPCCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	USavingSubsystem* savingSystem = GetGameInstance()->GetSubsystem<USavingSubsystem>();
	if (savingSystem) {
		savingSystem->AddNPCTempData(this);
	}
}

void ANPCCharacter::ExtendInteract()
{
}

void ANPCCharacter::ExtendStopInteract()
{
}

void ANPCCharacter::OnGameLoaded(UMansionSaveGame* Save)
{
	USavingSubsystem* savingSystem = GetGameInstance()->GetSubsystem<USavingSubsystem>();
	if (savingSystem) {
		savingSystem->GetNPCState(this, bShouldExist, bIsInteractible);
		SetActorHiddenInGame(!bShouldExist);
			SetActorEnableCollision(bShouldExist);
	}
	
}

#pragma region Interaction
void ANPCCharacter::StoppedTypingEvent()
{
	IInteractInterface::Execute_StopInteract(this, PlayerRef);
}
bool ANPCCharacter::GetIsInteractible_Implementation()
{
	return bIsInteractible;
}

void ANPCCharacter::SetIsInteractible_Implementation(const bool bToggle)
{
	bIsInteractible = bToggle;
}

bool ANPCCharacter::GetNeedsHold_Implementation()
{
	return bNeedsHold;
}

void ANPCCharacter::Interact_Implementation(AMyPlayerController* PlayerInstigating)
{
	//APlayerController* myPC = UGameplayStatics::GetPlayerController(PlayerInstigating, 0);
	//myPC->SetViewTargetWithBlend(this, CameraBlendTime, EViewTargetBlendFunction::VTBlend_Linear,1.f,false);
	if (!PlayerInstigating) return;
	if (DialogueComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Interacted with NPC"));
		if (DialogueComponent->HasDialogue()) {
			DialogueComponent->StartDialogue();
			//PlayerInstigating->ChangePlayerStatus(PlayerStatus::Interacting);
			PlayerRef = PlayerInstigating;
			PlayerInstigating->SetInputFocussedOn(this);
			bIsTalking = true;
			if (bChangeCamera) {
				APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
				if (CameraManager)
				{
					PreviousViewTarget = CameraManager->GetViewTarget();
					if (bBlendIn) {
						PlayerInstigating->SetViewTargetWithBlend(this, fBlendTime, BlendFunc, fBlendExp, false);
					}
					else {
						PlayerInstigating->SetViewTarget(this);
					}
				}
			}

		}
	}
	ExtendInteract();
	InteractedStarting();
}

/*void ANPCCharacter::ContinueInteract_Implementation(APlayerCharacter* PlayerInstigating)
{
	if (DialogueComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Continued Interact with NPC"));
		DialogueComponent->NextSentence();
	}
	else {
		IInteractInterface::Execute_StopInteract(this, PlayerRef);
	}
}*/

void ANPCCharacter::StopInteract_Implementation(AMyPlayerController* PlayerInstigating)
{
	if (PlayerInstigating) {
		PlayerInstigating->SetInputToPlayer();
		APlayerCharacter* myPlayer = Cast<APlayerCharacter>(PlayerInstigating->GetPawn());
		if (myPlayer) {
			myPlayer->CheckInteractibleInFront();
		}
		if (bChangeCamera) {
			APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
			if (CameraManager)
			{
				PlayerInstigating->SetViewTargetWithBlend(PreviousViewTarget, fBlendTime, BlendFunc, fBlendExp, false);
				PreviousViewTarget = nullptr;
			}
		}
		bIsTalking = false;
	}
	ExtendStopInteract();
	InteractedEnding();

	//PlayerInstigating->ChangePlayerStatus(PlayerStatus::Default);
	//PlayerRef = nullptr;
}

void ANPCCharacter::BackInteract_Implementation(AMyPlayerController* PlayerInstigating)
{

}
void ANPCCharacter::BottomFaceButton_Implementation()
{
	if (DialogueComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Continued Interact with NPC"));
		DialogueComponent->NextSentence();
	}
	else {
		IInteractInterface::Execute_StopInteract(this, PlayerRef);
	}
}
void ANPCCharacter::SetShouldExist(bool bVal)
{
	bShouldExist = bVal;
	SetActorHiddenInGame(!bVal);
	SetActorEnableCollision(bVal);
}
void ANPCCharacter::SetIsTalking(bool newVal)
{
	bIsTalking = newVal;
}
#pragma endregion




