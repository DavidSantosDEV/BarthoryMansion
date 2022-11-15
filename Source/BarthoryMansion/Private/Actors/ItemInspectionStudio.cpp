// Fropple Studios Copyright


#include "Actors/ItemInspectionStudio.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "MyComponents/InspectionCollisionComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BillboardComponent.h" 
#include "Interaction/InspectableItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h" 
#include "Particles/ParticleSystemComponent.h" 

// Sets default values
AItemInspectionStudio::AItemInspectionStudio()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ActorRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ActorRoot"));
	RootComponent = ActorRoot;

	CameraView = CreateDefaultSubobject<UCameraComponent>(TEXT("CaptureCamera"));
	CameraView->SetupAttachment(RootComponent);

	ItemLocation = CreateDefaultSubobject<UBillboardComponent>(TEXT("ItemPosition"));
	ItemLocation->SetupAttachment(RootComponent);

	myLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("ItemLight"));
	myLight->SetupAttachment(RootComponent);

	MyParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	MyParticles->SetupAttachment(RootComponent);
	MyParticles->bAutoActivate = false;

	fLineDistance = 10.f;
}

// Called when the game starts or when spawned
void AItemInspectionStudio::BeginPlay()
{
	Super::BeginPlay();

}

void AItemInspectionStudio::BeginDestroy()
{
	Super::BeginDestroy();
	if (currentItem) {
		currentItem->Destroy();
	}
}

void AItemInspectionStudio::ResetItemPositionAndRotation()
{
	if (currentItem) {
		currentItem->SetActorRotation(FRotator(0, 0, 0));
	}
}

void AItemInspectionStudio::MoveItemRight(float val)
{
	if (currentItem) {
		currentItem->AddActorWorldRotation(FRotator(0, -val * GetWorld()->DeltaTimeSeconds * fSpeedRotation, 0));
		//TraceForInteractions();
	}
}

void AItemInspectionStudio::MoveItemUp(float val)
{
	if (currentItem) {
		currentItem->AddActorWorldRotation(FRotator(-val * GetWorld()->DeltaTimeSeconds * fSpeedRotation, 0, 0));
		//TraceForInteractions();
	}
}

void AItemInspectionStudio::Inspect()
{
	TraceForInteractions();
	if (currentInspectionCollided) {
		currentInspectionCollided->Press();
	}
}

void AItemInspectionStudio::TraceForInteractions()
{
	if (currentItem && !this->IsPendingKill())
	{
		FVector startVector = CameraView->GetComponentLocation();

		FVector endVector = CameraView->GetForwardVector() * startVector * fLineDistance;

		FHitResult hitResult;
		if (GetWorld()->LineTraceSingleByChannel(hitResult, startVector, endVector, colTraceChannel)) {
			
			if(hitResult.Component->GetCollisionObjectType() == ObjectChannel){
				if (hitResult.Component != currentInspectionCollided) {
					UInspectionCollisionComponent* temp = Cast<UInspectionCollisionComponent>(hitResult.Component);
					if (temp) {
						currentInspectionCollided = temp;
						
						if (!currentInspectionCollided->GetAttachedWidget()) {
							if (WidgetCompClass) {
								UWidgetComponent* TempComp = Cast<UWidgetComponent>(hitResult.Actor->AddComponentByClass(WidgetCompClass, false, temp->GetComponentTransform(), true));
								if (TempComp) {
									currentInspectionCollided->SetAttachedWidget(TempComp);
									TempComp->SetWorldLocation(currentInspectionCollided->GetComponentLocation());
									TempComp->SetVisibility(true, true);
								}
							}
						}
						else {
							currentInspectionCollided->GetAttachedWidget()->SetVisibility(true, true);
						}	
						
						if (GEngine) {
							GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Yellow, TEXT("Item"));
						}
					}
				}
			}
			else
			{
				if (currentInspectionCollided) {
					if (currentInspectionCollided->GetAttachedWidget()) {
						currentInspectionCollided->GetAttachedWidget()->SetVisibility(false, false);
					}
				}
				currentInspectionCollided = nullptr;
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Red, TEXT("Nothing"));
				}
			}
			/*if (hitResult.Component != currentInspectionCollided) {
				UInspectionCollisionComponent* temp = Cast<UInspectionCollisionComponent>(hitResult.Component);
				if (temp) {
					currentInspectionCollided = temp;
					if (GEngine) {
						GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Yellow, TEXT("Item"));
					}
				}
				else
				{
					currentInspectionCollided = nullptr;
					if (GEngine) {
						GEngine->AddOnScreenDebugMessage(-1, .1f, FColor::Red, TEXT("Nothing"));
					}
				}
			}*/
			

		}
	}
	
}

void AItemInspectionStudio::SetCurrentItem(TSubclassOf<AInspectableItem>newItem)
{
	if (newItem) {
		if (currentItem) {
			currentItem->Destroy();
		}
		const FActorSpawnParameters param;
		FVector Location= ItemLocation->GetComponentLocation();
		

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		currentItem = GetWorld()->SpawnActor<AInspectableItem>(newItem, Location, ItemInitialRotation, SpawnParams);
		if (currentItem) {
			currentItem->SetStudio(this);
		}
	}
}

void AItemInspectionStudio::StartTracingForItem()
{
	//GetWorldTimerManager().SetTimer(thTraceLine, this, &AItemInspectionStudio::TraceForInteractions, fTraceTimePause, true, 0);
	MyParticles->Activate();
	APlayerController* PController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PController) {
		PreviousViewTarget = PController->GetViewTarget();
		PController->SetViewTarget(this);
	}

}

void AItemInspectionStudio::StopTracingForItem()
{
	MyParticles->Deactivate();
	if (PreviousViewTarget) {
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTarget(PreviousViewTarget);
	}
	/*if (GetWorldTimerManager().IsTimerActive(thTraceLine)) {
		GetWorldTimerManager().ClearTimer(thTraceLine);
	}*/
	if (currentItem) {
		currentItem->Destroy();
	}
}
