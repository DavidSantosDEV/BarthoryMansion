// Fropple Studios Copyright


#include "Interaction/ItemUsingInteractible.h"
#include "MyPlayerController.h"
#include "Inventory/InventoryComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h" 
#include "Blueprint/UserWidget.h"
#include "Inventory/Item.h"
#include "MyHUD.h"

// Sets default values
AItemUsingInteractible::AItemUsingInteractible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	myMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Mesh"));
	RootComponent = myMesh;
	CameraView = CreateDefaultSubobject<UCameraComponent>(FName("CameraView"));
	CameraView->SetupAttachment(RootComponent);
}

void AItemUsingInteractible::Interact_Implementation(AMyPlayerController* PlayerInstigating)
{
	if (IsValid(RequiredItemClass)) {
		AMyHUD* mHud = Cast<AMyHUD>(PlayerInstigating->GetHUD());
		if (mHud) {
			mHud->SetInventoryNeededItem(RequiredItemClass, this);
			mHud->ShowInventoryWidget();
			//PlayerInstigating->SetInputFocussedOn(mHud->GetInventoryWidget(), true);
			FInputModeUIOnly InputMode;
			//InputMode.SetWidgetToFocus(mHud->GetInventoryWidget());
			PlayerInstigating->SetInputMode(InputMode);
			PreviousView = PlayerInstigating->GetViewTarget();
			PlayerInstigating->SetViewTarget(this);
			APawn* playerPawn = PlayerInstigating->GetPawn();
			if (playerPawn) {
				playerPawn->SetActorHiddenInGame(true);
			}
		}
	}

}

void AItemUsingInteractible::ItemUsed()
{
	AMyPlayerController* pController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!pController) return;
	pController->SetViewTarget(PreviousView);
	AMyHUD* mHud = Cast<AMyHUD>(pController->GetHUD());
	if (mHud) {
		mHud->HideCurrentShowPrevious();
	}
	
	//pController->SetInputToPlayer();
	bIsInteractible = false;
	if (bRemoveItem) {
		APlayerCharacter* playerChar = Cast<APlayerCharacter>(pController->GetCharacter());
		if (playerChar) {
			playerChar->SetActorHiddenInGame(false);
			playerChar->GetInventoryComponent()->RemoveItemOfClass(RequiredItemClass);
		}
	}
	OnItemUsed();
}

void AItemUsingInteractible::ResetPlayerView()
{
	AMyPlayerController* pController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!pController) return;
	pController->SetViewTarget(PreviousView);
	APawn* playerPawn = pController->GetPawn();
	if (playerPawn) {
		playerPawn->SetActorHiddenInGame(false);
	}
}

void AItemUsingInteractible::OnItemUsed_Implementation()
{
}
