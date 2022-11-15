// Fropple Studios Copyright


#include "TogglePuzzleTorch.h"

// Sets default values
ATogglePuzzleTorch::ATogglePuzzleTorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ATogglePuzzleTorch::BeginPlay()
{
	Super::BeginPlay();
	bIsToggled = bStartToggled;
	SetStateAsOriginal();
	//OnToggleUpdateVisuals();
}

void ATogglePuzzleTorch::Toggle()
{
	ToggleDescrete();
	for (ATogglePuzzleTorch* Tog : ActorsToToggle) {
		if (Tog) {
			Tog->ToggleDescrete();
		}
	}
	OnToggleDelegate.Broadcast();
}

void ATogglePuzzleTorch::ToggleDescrete()
{
	bIsToggled = !bIsToggled;
	OnToggleUpdateVisuals();
}

bool ATogglePuzzleTorch::GetIsInteractible_Implementation()
{
	return bIsActive;
}

EInteractibleType ATogglePuzzleTorch::GetInteractionType_Implementation()
{
	return EInteractibleType::EOneClick;
}

void ATogglePuzzleTorch::Interact_Implementation(AMyPlayerController* PlayerInstigating)
{
	Toggle();
}
