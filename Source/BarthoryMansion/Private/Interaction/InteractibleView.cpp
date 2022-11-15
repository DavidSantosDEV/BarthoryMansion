// Fropple Studios Copyright


#include "Interaction/InteractibleView.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "MyHUD.h"

AInteractibleView::AInteractibleView()
{
	InteractionRoot = CreateDefaultSubobject<USceneComponent>(FName("InteractionRoot"));
	RootComponent = InteractionRoot;

	InteractionCollision = CreateDefaultSubobject<UBoxComponent>(FName("Interaction Collision"));
	InteractionCollision->SetupAttachment(RootComponent);

	CameraView = CreateDefaultSubobject<UCameraComponent>(FName("Camera View"));
	CameraView->SetupAttachment(RootComponent);
}

void AInteractibleView::Interact_Implementation(AMyPlayerController* PlayerInstigating)
{
	if (PlayerInstigating) {
		CurrentPlayer = PlayerInstigating;
		PreviousPlayerView = PlayerInstigating->GetViewTarget();
		PlayerInstigating->SetViewTargetWithBlend(this, BlendTime, BlendFunction, BlendExp, false);
		PlayerInstigating->SetInputFocussedOn(this);
		AMyHUD* MyHud = Cast<AMyHUD>( PlayerInstigating->GetHUD());
		if (MyHud) {
			MyHud->ToggleViewUI(true);
		}
	}
}

void AInteractibleView::StopInteract_Implementation(AMyPlayerController* PlayerInstigating)
{
	if (!CurrentPlayer)return;
	CurrentPlayer->SetViewTargetWithBlend(PreviousPlayerView, BlendTime, BlendFunction, BlendExp);
	CurrentPlayer->SetInputToPlayer();
	if (PlayerInstigating) {
		AMyHUD* MyHud = Cast<AMyHUD>(PlayerInstigating->GetHUD());
		if (MyHud) {
			MyHud->ToggleViewUI(false);
		}
	}
	
}

void AInteractibleView::BackInteract_Implementation(AMyPlayerController* PlayerInstigating)
{
}

void AInteractibleView::LeftAnalogForward_Implementation(float Value)
{
	FRotator Rotation = CameraView->GetRelativeRotation();
	Rotation.Pitch = UKismetMathLibrary::FInterpTo(Rotation.Pitch, Value * CameraMaxPitch, GetWorld()->DeltaTimeSeconds, InterpSpeed);
	CameraView->SetRelativeRotation(Rotation);
}

void AInteractibleView::LeftAnalogRight_Implementation(float Value)
{
	FRotator Rotation = CameraView->GetRelativeRotation();
	
	Rotation.Yaw = UKismetMathLibrary::FInterpTo(Rotation.Yaw, Value * CameraMaxYaw, GetWorld()->DeltaTimeSeconds,InterpSpeed);

	CameraView->SetRelativeRotation(Rotation);
}

void AInteractibleView::RightFaceButton_Implementation()
{
	if (!PreviousPlayerView) return;
	CurrentPlayer = CurrentPlayer == nullptr ? Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)) : CurrentPlayer;
	if (!CurrentPlayer) {
		return;
	}
	IInteractInterface::Execute_StopInteract(this, CurrentPlayer);
}
