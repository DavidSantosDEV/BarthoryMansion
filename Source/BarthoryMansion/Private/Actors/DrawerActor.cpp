// Fropple Studios Copyright


#include "Actors/DrawerActor.h"
#include "Components/TimelineComponent.h" 
#include "MyPlayerController.h"
#include "MyComponents/DialogueComponent.h"
#include "Subsystems/SavingSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerCharacter.h"
#include "Inventory/InventoryComponent.h"

// Sets default values
ADrawerActor::ADrawerActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MainDrawerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MainDrawer"));
	RootComponent = MainDrawerMesh;

	RightDrawer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDrawer"));
	RightDrawer->SetupAttachment(RootComponent);

	LeftDrawer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDrawer"));
	LeftDrawer->SetupAttachment(RootComponent);


	DialogueComp = CreateDefaultSubobject<UDialogueComponent>(TEXT("DialogueComponent"));
}

// Called when the game starts or when spawned
void ADrawerActor::BeginPlay()
{
	Super::BeginPlay();

	DialogueComp->OnStopDialogueEvent.AddDynamic(this, &ADrawerActor::OnDialogueFinished);

	if (FloatCurve != NULL)
	{
		FOnTimelineFloat onTimelineCallback;
		FOnTimelineEventStatic onTimelineFinishedCallback;

		MyTimeline = NewObject<UTimelineComponent>(this, FName("OpeningTimeline"));
		MyTimeline->CreationMethod = EComponentCreationMethod::UserConstructionScript; // Indicate it comes from a blueprint so it gets cleared when we rerun construction scripts
		this->BlueprintCreatedComponents.Add(MyTimeline); // Add to array so it gets saved
		//MyTimeline->SetNetAddressable();	// This component has a stable name that can be referenced for replication

		MyTimeline->SetPropertySetObject(this); // Set which object the timeline should drive properties on
		MyTimeline->SetDirectionPropertyName(FName("TimelineDirection"));

		MyTimeline->SetLooping(false);
		MyTimeline->SetTimelineLength(OpeningTime);
		MyTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_LastKeyFrame);

		MyTimeline->SetPlaybackPosition(0.0f, false);

		//Add the float curve to the timeline and connect it to your timelines's interpolation function
		
		onTimelineCallback.BindUFunction(this, FName{ TEXT("OnTimelimeTick") });
		onTimelineFinishedCallback.BindUFunction(this, FName{ TEXT("OnTimelineFinished") });

		MyTimeline->AddInterpFloat(FloatCurve, onTimelineCallback);

		MyTimeline->SetTimelineFinishedFunc(onTimelineFinishedCallback);

		MyTimeline->RegisterComponent();
	}

	USavingSubsystem* savingSystem = GetGameInstance()->GetSubsystem<USavingSubsystem>();
	if (savingSystem) {
		savingSystem->OnGameLoadedSave.AddDynamic(this, &ADrawerActor::OnGameLoaded);
	}

}

void ADrawerActor::OpenDrawer()
{
	if (MyTimeline) {
		StatusOf = Opening;
		MyTimeline->PlayFromStart();
	}
}

void ADrawerActor::CloseDrawer()
{
	if (MyTimeline) {
		StatusOf = Closing;
		MyTimeline->ReverseFromEnd();
	}
}

void ADrawerActor::OnTimelimeTick(float deltaValue)
{
	//MoveThings
	float value = FloatCurve->GetFloatValue(MyTimeline->GetPlaybackPosition()) * OpenAngle;
	RightDrawer->SetRelativeRotation(FRotator(RightDrawer->GetRelativeRotation().Pitch, -value , RightDrawer->GetRelativeRotation().Roll));
	LeftDrawer->SetRelativeRotation(FRotator(LeftDrawer->GetRelativeRotation().Pitch, value, LeftDrawer->GetRelativeRotation().Roll));
}

void ADrawerActor::OnTimelineFinished()
{
	if (StatusOf == EDrawerStatus::Opening) {
		StatusOf = EDrawerStatus::Open;
	}
	else {
		StatusOf = EDrawerStatus::Closed;
	}
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 4.f, FColor::Emerald, FString::Printf(TEXT("DrawerStatus: %s"), *UEnum::GetValueAsString(StatusOf.GetValue())));
	}
}

void ADrawerActor::OnDialogueFinished()
{
	UWorld* const World = GetWorld();
	if (World) {
		AMyPlayerController* PlayerC = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(World, 0));
		if (PlayerC) {
			PlayerC->SetInputToPlayer();
		}
	}
}

void ADrawerActor::OnGameLoaded(UMansionSaveGame* SaveData)
{
	USavingSubsystem* savingSystem = GetGameInstance()->GetSubsystem<USavingSubsystem>();
	if (savingSystem) {
		
		savingSystem->GetDrawerState(this, bHasGivenDisc, StatusOf);
		SetDrawerStatus(StatusOf, bHasGivenDisc);
	}
}

// Called every frame
void ADrawerActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MyTimeline) {
		MyTimeline->TickComponent(DeltaTime,ELevelTick::LEVELTICK_TimeOnly,NULL);
	}
}

bool ADrawerActor::GetIsInteractible_Implementation()
{
	return !(StatusOf == EDrawerStatus::Opening || StatusOf == EDrawerStatus::Closing);
}

EInteractibleType ADrawerActor::GetInteractionType_Implementation()
{
	return EInteractibleType::EOneClick;
}

void ADrawerActor::Interact_Implementation(AMyPlayerController* PlayerInstigating)
{
	if (PlayerInstigating) {
		switch (StatusOf)
		{
		case EDrawerStatus::Closed:
		{
			//Open Drawer

			UDataTable* ChosenDialogue = NoItemDiscDialogue;
			if (!bHasGivenDisc && MusicItem.MusicEvent != nullptr) {
				APawn* PlayerPawn = PlayerInstigating->GetPawn();
				if (PlayerPawn) {
					APlayerCharacter* Player = Cast<APlayerCharacter>(PlayerPawn);
					if (Player) {

						Player->GetInventoryComponent()->AddMusic(MusicItem);
						bHasGivenDisc = true;

						ChosenDialogue = GiveDiscDialogue;
					}
				}
			}
			if (ChosenDialogue) {
				DialogueComp->ChangeDialogueData(ChosenDialogue);
				DialogueComp->StartDialogue();
				PlayerInstigating->SetInputFocussedOn(this);

				OpenDrawer();
			}
			break;
		}
		default:
		case EDrawerStatus::Closing:
		case EDrawerStatus::Opening:
			return;
			break;
		case EDrawerStatus::Open:
			CloseDrawer();
			break;
		}
	}
}

void ADrawerActor::BottomFaceButton_Implementation()
{
	DialogueComp->NextSentence();
}

void ADrawerActor::SetDrawerStatus(EDrawerStatus newStatus, bool bNewHasDisc)
{
	float value = OpenAngle;
	if (EDrawerStatus::Closed == newStatus || EDrawerStatus::Closing == newStatus) {
		value = 0.f;
	}

	RightDrawer->SetRelativeRotation(FRotator(RightDrawer->GetRelativeRotation().Pitch, -value, RightDrawer->GetRelativeRotation().Roll));
	LeftDrawer->SetRelativeRotation(FRotator(LeftDrawer->GetRelativeRotation().Pitch, value, LeftDrawer->GetRelativeRotation().Roll));

	bHasGivenDisc = bNewHasDisc;

}

TEnumAsByte<EDrawerStatus> ADrawerActor::GetDrawerStatus()
{
	if (StatusOf == Closing) return Closed;
	if (StatusOf == Opening) return Open;
	return StatusOf;
}

