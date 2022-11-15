// Fropple Studios Copyright


#include "Actors/BoilerPuzzleBase.h"
#include "Components/StaticMeshComponent.h" 
#include "Camera/CameraComponent.h"
#include "MyPlayerController.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/SplineFollowCamera.h"
#include "CHaracters/PlayerCharacter.h"
#include "MyComponents/DialogueComponent.h"
#include "FMODBlueprintStatics.h"
#include "NiagaraComponent.h"

ABoilerPuzzleBase::ABoilerPuzzleBase()
{
	MainBoilerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boiler Mesh"));
	RootComponent = MainBoilerMesh;

	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Boiler Plate"));
	PlateMesh->SetupAttachment(RootComponent);

	ParticleBomb = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Explosion Particle"));
	ParticleBomb->bAutoActivate = false;

	ParticleBomb->SetupAttachment(RootComponent);

	ScrewOne = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screw 1"));
	ScrewOne->SetupAttachment(PlateMesh);

	ScrewTwo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screw 2"));
	ScrewTwo->SetupAttachment(PlateMesh);

	ScrewThree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screw 3"));
	ScrewThree->SetupAttachment(PlateMesh);

	ScrewFour = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Screw 4"));
	ScrewFour->SetupAttachment(PlateMesh);

	CondensorOne = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Condensor 1"));
	CondensorOne->SetupAttachment(RootComponent);
	
	CondensorTwo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Condensor 2"));
	CondensorTwo->SetupAttachment(RootComponent);
		
	CondensorThree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Condensor 3"));
	CondensorThree->SetupAttachment(RootComponent);
	
	CondensorFour = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Condensor 4"));
	CondensorFour->SetupAttachment(RootComponent);

	CameraView = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera View"));
	CameraView->SetupAttachment(RootComponent);

	DialogueComp = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));

}

void ABoilerPuzzleBase::BeginPlay()
{
	Super::BeginPlay();
	if (bShouldExist) {
		CondensorList.AddUnique(CondensorOne);
		CondensorList.AddUnique(CondensorTwo);
		CondensorList.AddUnique(CondensorThree);
		CondensorList.AddUnique(CondensorFour);
		for (int i = 0; i < CondensorList.Num(); ++i) {
			CondensorList[i]->bRenderCustomDepth = true;
			FLocationAndIndex Loc;
			Loc.Comp = CondensorList[i];
			Loc.Index = i;
			Loc.Location = CondensorList[i]->GetRelativeLocation();
			ComponentStartingLocation.Add(Loc);
		}

		GenerateConLocation();

		DialogueComp->OnStopDialogueEvent.AddDynamic(this, &ABoilerPuzzleBase::OnDialogueIsFinished);
	}
}

void ABoilerPuzzleBase::GenerateConLocation()
{
	for (int i = 0; i < CondensorList.Num(); ++i) {
		if (OrderArray.IsValidIndex(i)) {
			ComponentDesiredLocation.Add(OrderArray[i], CondensorList[i]);
		}
		else {
			bIsInteractible = false;
			return;
		}
	}
}

void ABoilerPuzzleBase::CheckLocations()
{
	for (int i = 0; i < CondensorList.Num(); ++i) {
		UStaticMeshComponent* Comp = ComponentDesiredLocation.FindRef(i);
		if (Comp) {
			if (Comp != CondensorList[i]) {
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, FString::Printf(TEXT("Index value %d returned false"), i));
				}
				return;
			}
			if (GEngine) {
				GEngine->AddOnScreenDebugMessage(-1, 3.5f, FColor::Red, FString::Printf(TEXT("Index value %d returned true"), i));
			}
		}
	}
	BlowUp();
}

void ABoilerPuzzleBase::MoveIndex(int indexVal)
{
	if (bPlateOpen) {
		int currentTemp = CurrentIndex + indexVal;
		if (currentTemp > 3) {
			currentTemp = 0;
		}
		if (currentTemp < 0) {
			currentTemp = 3;
		}
		SelectComp(CurrentIndex, 0);
		CurrentIndex = currentTemp;
		SelectComp(CurrentIndex, 1);
		UFMODBlueprintStatics::PlayEvent2D(this, SelectionSound, true);
		//UGameplayStatics::PlaySound2D(GetWorld(), SelectionSound, 1, 1, 0, nullptr, this, true);
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(0, 3.5f, FColor::Red, FString::Printf(TEXT(" Index value %d"), CurrentIndex));
		}
	}
}

void ABoilerPuzzleBase::SelectComp(int index, float val)
{
	if (!bCanSelect) return;
	if (CondensorList.IsValidIndex(index)) {
		if (bHasSelectedIndex && index == FirstSelectedIndex) {
			return;
		}
		UMaterialInstanceDynamic* DynamicInstance = CondensorList[index]->CreateDynamicMaterialInstance(0, MaterialInstance);
		if (DynamicInstance) {
			DynamicInstance->SetScalarParameterValue(SelectedParameterName, val);
		}
	}
	
}

void ABoilerPuzzleBase::BlowUp()
{
	RemoveUI();
	if (bShouldExist) {
		bIsInteractible = false;
		bShouldExist = false;
		bIsCompleted = true;
		AMyPlayerController* PlayerCont = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (PlayerCont)
		{
			ASplineFollowCamera* CameraCasted = Cast<ASplineFollowCamera>(PreviousViewTarget);
			if (CameraCasted) {
				CameraCasted->ToggleCamera(true);
			}
			else {
				PlayerCont->SetViewTarget(PreviousViewTarget);
			}
			//PlayerCont->SetInputToPlayer();
			APawn* PlayerPawn = PlayerCont->GetPawn();
			if (PlayerPawn) {
				PlayerPawn->SetActorHiddenInGame(false);
			}

		}
		bIsCompleted = true;
		bDialogueMode=true;
		DialogueComp->ChangeDialogueData(DialogueFinish);
		if (DialogueComp->HasDialogue()) {		
			DialogueComp->StartDialogue();
		}
		else {
			OnDialogueIsFinished();
		}
		OnPuzzleFinished();
	}
}

void ABoilerPuzzleBase::ResetPuzzle()
{
	bHasSelectedIndex = false;
	for (int i = 0; i < CondensorList.Num(); ++i) {
		SelectComp(i, -1);
	}
	bCanSelect = false;
}

void ABoilerPuzzleBase::OnDialogueIsFinished()
{
	UWorld* const World = GetWorld();
	if (World) {
		AMyPlayerController* PC = (AMyPlayerController*)UGameplayStatics::GetPlayerController(World, 0);
		if (PC) {
			PC->SetInputToPlayer();
		}
	}

}

void ABoilerPuzzleBase::ActualBlowUp()
{
	SetActorEnableCollision(false);
	MainBoilerMesh->SetVisibility(false, false);
	PlateMesh->SetVisibility(false, true);
	for (int i = 0; i < CondensorList.Num(); ++i) {
		CondensorList[i]->SetVisibility(false);
	}
	ParticleBomb->Activate();
	UFMODBlueprintStatics::PlayEventAtLocation(this, ExplosionSound, GetActorTransform(), true);
	//UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	FTimerHandle timerH;
	GetWorld()->GetTimerManager().SetTimer(timerH, this, &ABoilerPuzzleBase::StartSecondDialogue, TimeForExplosion, false);
}

void ABoilerPuzzleBase::StartSecondDialogue()
{
	AMyPlayerController* PController = (AMyPlayerController*)UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PController) {
		PController->SetInputFocussedOn(this);
		DialogueComp->ChangeDialogueData(SecondDialogue);
		DialogueComp->StartDialogue();
	}
}

void ABoilerPuzzleBase::InitiatePuzzle()
{
	CreateUI();
	AMyPlayerController* PlayerInstigating = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	CurrentIndex = 0;
	PreviousViewTarget = PlayerInstigating->PlayerCameraManager->GetViewTarget();
	PlayerInstigating->SetViewTarget(this);
	PlayerInstigating->SetAudioListenerOverride(CameraView, FVector::ZeroVector, FRotator::ZeroRotator);
	PlayerInstigating->SetInputFocussedOn(this);
	APawn* PlayerPawn = PlayerInstigating->GetPawn();
	if (PlayerPawn) {
		PlayerPawn->SetActorHiddenInGame(true);
	}

	for (int i = 0; i < ComponentStartingLocation.Num(); ++i) {
		CondensorList[ComponentStartingLocation[i].Index] = ComponentStartingLocation[i].Comp;
		CondensorList[ComponentStartingLocation[i].Index]->SetRelativeLocation(ComponentStartingLocation[i].Location);
	}

	SelectComp(CurrentIndex, 1);
}

void ABoilerPuzzleBase::QuitPuzzle()
{
	RemoveUI();
	AMyPlayerController* PlayerCont = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerCont)
	{
		ASplineFollowCamera* CameraCasted = Cast<ASplineFollowCamera>(PreviousViewTarget);
		if (CameraCasted) {
			CameraCasted->ToggleCamera(true);
		}
		else {
			PlayerCont->SetViewTarget(PreviousViewTarget);
		}
		
		PlayerCont->SetInputToPlayer();
		APawn* PlayerPawn = PlayerCont->GetPawn();
		if (PlayerPawn) {
			PlayerPawn->SetActorHiddenInGame(false);
		}
	}
}

void ABoilerPuzzleBase::Interact_Implementation(AMyPlayerController* PlayerInstigating)
{
	if (bIsCompleted)return;
	if (PlayerInstigating) {
		APlayerCharacter* player = Cast<APlayerCharacter>(PlayerInstigating->GetPawn());
		if (player) {
			if (!player->GetHasLanternOut()) {
				DialogueComp->ChangeDialogueData(DialogueNoLight);
				DialogueComp->StartDialogue();
				PlayerInstigating->SetInputFocussedOn(this);
				bDialogueMode = true;
			}
			else {
				bDialogueMode = false;
				InitiatePuzzle();
				OpenPlate();
				bCanSelect = true;
				SelectComp(CurrentIndex, 1);
			}
		}
		
	}
}

void ABoilerPuzzleBase::BottomFaceButton_Implementation()
{
	if (bDialogueMode) {
		DialogueComp->NextSentence();
	}
	else {
		if (bCanSelect && bPlateOpen) {
			if (CondensorList.IsValidIndex(CurrentIndex)) {
				if (bHasSelectedIndex) {
					if (CurrentIndex == FirstSelectedIndex) {
						bHasSelectedIndex = false;
						SelectComp(CurrentIndex, 1); //HEREEEEE
					}
					else {
						FVector positionOther = CondensorList[CurrentIndex]->GetRelativeLocation();

						CondensorList[CurrentIndex]->SetRelativeLocation(CondensorList[FirstSelectedIndex]->GetRelativeLocation(), false, nullptr, ETeleportType::ResetPhysics);
						CondensorList[FirstSelectedIndex]->SetRelativeLocation(positionOther, false, nullptr, ETeleportType::ResetPhysics);

						UStaticMeshComponent* Middleman = CondensorList[CurrentIndex];
						CondensorList[CurrentIndex] = CondensorList[FirstSelectedIndex];
						CondensorList[FirstSelectedIndex] = Middleman;
						SelectComp(CurrentIndex, 1);
						bHasSelectedIndex = false;
						SelectComp(FirstSelectedIndex, 0);

						CheckLocations();
					}
				}
				else {
					bHasSelectedIndex = true;
					FirstSelectedIndex = CurrentIndex;
					SelectComp(CurrentIndex, 1);
				}
			}
		}
		
	}
	

}



void ABoilerPuzzleBase::RightFaceButton_Implementation()
{
	if (bCanSelect && bPlateOpen) {
		ResetPuzzle();
		ClosePlate();
	}
	//Leave
}

void ABoilerPuzzleBase::DPadRight_Implementation()
{
	MoveIndex(1);
}

void ABoilerPuzzleBase::DPadLeft_Implementation()
{
	MoveIndex(-1);
}


