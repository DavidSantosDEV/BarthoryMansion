// Fropple Studios Copyright


#include "Actors/SplineFollowCamera.h"
#include "Camera/CameraComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetMathLibrary.h" 
#include "Components/BillboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyPlayerController.h"
#include "Subsystems/CameraManagerSubsystem.h"
#include "Components/BoxComponent.h"

// Sets default values
ASplineFollowCamera::ASplineFollowCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//PrimaryActorTick.bStartWithTickEnabled = false;

	BillboardIcon = CreateDefaultSubobject<UBillboardComponent>(FName("CameraIcon"));
	SetRootComponent(BillboardIcon);

	CameraView = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	CameraView->SetupAttachment(RootComponent);

	SplineCamera = CreateDefaultSubobject<USplineComponent>(FName("CameraSpline"));
	SplineCamera->SetupAttachment(RootComponent);

	BoundsBox = CreateDefaultSubobject<UBoxComponent>(FName("Bounds"));
	BoundsBox->SetupAttachment(RootComponent);

	BoundsBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASplineFollowCamera::HandleBeginOverlap); //If no work, add to begin play

	RotationType = ECameraRotationType::ENone;
}

// Called when the game starts or when spawned
void ASplineFollowCamera::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickEnabled(false);
}

void ASplineFollowCamera::SetCameraLocation()
{
	if (Player) {
		FVector TargetLocation = SplineCamera->FindLocationClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);
		TargetLocation += CameraOffset;
		CameraView->SetWorldLocation(FMath::VInterpTo(CameraView->GetComponentLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), fInterpTime));
	}
}

void ASplineFollowCamera::SetCameraRotation()
{
	if (Player) {
		switch (RotationType)
		{
		case ECameraRotationType::EFollowPlayer:
		{
			FRotator PlayerLook = UKismetMathLibrary::FindLookAtRotation(CameraView->GetComponentLocation(), Player->GetActorLocation());
			FRotator middleRot = CameraView->GetComponentRotation();

			//Equivalent of 3 ifs, or 3 float num with this
			middleRot = FRotator(bRotateY ? PlayerLook.Pitch : middleRot.Pitch, bRotateZ ? PlayerLook.Yaw : middleRot.Yaw, /*bRotateX ? PlayerLook.Roll :*/ middleRot.Roll);
			middleRot = FMath::RInterpTo(CameraView->GetComponentRotation(), middleRot, GetWorld()->GetDeltaSeconds(), fRotationInterpSpeed);
			CameraView->SetWorldRotation(middleRot);

			break;
		}

		case ECameraRotationType::EFollowSpline:
		{
			FRotator target = SplineCamera->FindRotationClosestToWorldLocation(Player->GetActorLocation(), ESplineCoordinateSpace::World);
			CameraView->SetWorldRotation(target);
			break;
		}
		default:
		case ECameraRotationType::ENone:
			break;
		}
		CameraView->AddWorldRotation(RotationOffset);
	}
}

void ASplineFollowCamera::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Triggered Cam"));
	if (IsActorTickEnabled()) {
		UE_LOG(LogTemp, Warning, TEXT("Was Enabled Before"));
		return;
	}
	APlayerCharacter* midPlayer = Cast<APlayerCharacter>(OtherActor);
	if (midPlayer) {
		Player = midPlayer;
		ToggleCamera(true);
		UE_LOG(LogTemp, Warning, TEXT("Was Toggled"));
	}

}

void ASplineFollowCamera::BecomeViewTarget(APlayerController* PC)
{
	AMyPlayerController* PlayerControl = Cast<AMyPlayerController>(PC);
	if (PlayerControl) {
		PlayerControl->SetNewSplineCamera(this);
	}
}

// Called every frame
void ASplineFollowCamera::Tick(float DeltaTime) //May or may not be needed
{
	Super::Tick(DeltaTime);

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Purple, FString::Printf(TEXT("Current Active Camera: %s"), *GetName()));
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Purple, FString::Printf(TEXT("Camera Rotation: %s"), *CameraView->GetComponentRotation().ToString()));
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Purple, FString::Printf(TEXT("Current Location: %s"), *CameraView->GetComponentLocation().ToString()));
		//GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Purple, FString::Printf(TEXT("Camera Settings"), GetName()));
	}
	if (bCameraMoveAlongSpline) {
		SetCameraLocation();
	}
	SetCameraRotation();
}

void ASplineFollowCamera::ToggleCamera(bool bActive)
{
	if (bActive) {
		APlayerController* PlayerC;
		if (!Player) {
			Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			if (!Player) { return; }
		}
		PlayerC = Cast<APlayerController>(Player->GetController());
		ASplineFollowCamera* PreviousSplineCam = Cast<ASplineFollowCamera>(PlayerC->GetViewTarget());
		if (PreviousSplineCam) {
			PreviousSplineCam->ToggleCamera(false);
			if (bForceBlendTime) {
				PlayerC->SetViewTargetWithBlend(this, fBlendTime, BlendFunc, fBlendExp, false);
			}
			else {
				if (PreviousSplineCam->GetBlendTimeOut() > 0.f || PreviousSplineCam->GetForcelendsOut()) {
					PlayerC->SetViewTargetWithBlend(this, PreviousSplineCam->GetBlendTimeOut(), PreviousSplineCam->GetBlendFuncOut(), PreviousSplineCam->GetBlendExpOut(), false);
				}
				else {
					PlayerC->SetViewTargetWithBlend(this, fBlendTime, BlendFunc, fBlendExp, false);
				}
			}
		}
		else {
			PlayerC->SetViewTargetWithBlend(this, fBlendTime, BlendFunc, fBlendExp, false);
		}
		PlayerC->SetAudioListenerOverride(CameraView, FVector::ZeroVector, FRotator::ZeroRotator);
		SetActorTickEnabled(true);
		UCameraManagerSubsystem* CameraManager = GetWorld()->GetSubsystem<UCameraManagerSubsystem>();
		if (CameraManager) {
			CameraManager->SetPlayerCamera(this);
		}
	}
	else {
		SetActorTickEnabled(false);
	}
	OnCameraToggledDelegate.Broadcast(bActive);
}

