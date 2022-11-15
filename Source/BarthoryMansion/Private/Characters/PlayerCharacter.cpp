// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "MyInterfaces/InteractInterface.h"
#include "MyPlayerController.h"
#include "MyHUD.h"
#include "Camera/PlayerCameraManager.h" 
#include "Inventory/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Actors/PlayerLantern.h"
#include "MyPlayerController.h"
#include "MyComponents/IKFootTracingComponent.h"
#include "Subsystems/SavingSubsystem.h"
#include "Components/WidgetComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	/*CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true;  Rotate the arm based on the controller*/

	// Create a follow camera
	/*FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;*/

	//Setup interaction collision
	InteractionCollision = CreateDefaultSubobject<UBoxComponent>(FName("InteractionReach"));
	InteractionCollision->InitBoxExtent(FVector(30.f, 30.f,30.f));
	InteractionCollision->SetupAttachment(GetMesh());

	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("PlayerInventory"));

	WidgetInteract = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	WidgetInteract->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetInteract->SetupAttachment(GetMesh());
	WidgetInteract->SetRelativeLocation(FVector(0, 0, 190));

	FootIKComp = CreateDefaultSubobject<UIKFootTracingComponent>(TEXT("FootIK"));
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	InteractionCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &APlayerCharacter::BeginOverlapInteract);
	InteractionCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &APlayerCharacter::EndOverlapInteract);
	//UE4 Doesnt like that put this on the constructor for some odd reason 

	CheckInteractibleInFront();

	if (bHasLantern) {
		CreateLantern();
	}

	myController = Cast<AMyPlayerController>(GetController());


	USavingSubsystem* SavingSys = GetGameInstance()->GetSubsystem<USavingSubsystem>();
	if (SavingSys) {
		if (SavingSys->GetCurrentTempData()) {
			SavingSys->LoadPlayerData();
		}
	}
}

#pragma region Interaction

void APlayerCharacter::BeginOverlapInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OtherActor->GetName());
	//IInteractInterface* Interactible = Cast<IInteractInterface:>(OtherActor->GetComponentByClass(UInteractComponent::StaticClass()));
	if (OtherActor->Implements<UInteractInterface>())
	{
		bool Interactible = IInteractInterface::Execute_GetIsInteractible(OtherActor);
		if (Interactible) {
			InteractibleSelected = OtherActor;
			WidgetInteract->SetVisibility(true, true);
			UE_LOG(LogTemp, Warning, TEXT("Added Interactible"));
		}
	}

}
void APlayerCharacter::EndOverlapInteract(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//UInteractComponent* Interactible = Cast<UInteractComponent>(OtherActor->GetComponentByClass(UInteractComponent::StaticClass()));

	if (OtherActor == InteractibleSelected) {
		InteractibleSelected = nullptr;
		//TODO
		// Check if Actor has mesh and try and remove highlight!
		UE_LOG(LogTemp, Warning, TEXT("Removed Interactible"));

		WidgetInteract->SetVisibility(false, true);
		
	}
	
}

void APlayerCharacter::CheckInteractibleInFront()
{
	TArray<AActor*> Result;
	InteractionCollision->GetOverlappingActors(Result);
	for (int i = 0; i < Result.Num(); ++i) {
		if (Result[i]->GetClass()->ImplementsInterface(UInteractInterface::StaticClass())) {
			WidgetInteract->SetVisibility(true, true);
			InteractibleSelected = Result[i];
			return;
		}
	}
	WidgetInteract->SetVisibility(false, true);
}

/*void APlayerCharacter::SetCurrentCamera(UCameraComponent* NewCameraView)
{
	if (NewCameraView) {
		NewCurrentCameraView = NewCameraView;
		UE_LOG(LogTemp,Warning, TEXT("New Camera = &s"), *NewCameraView->GetName())
	}
}*/


void APlayerCharacter::Interact()
{
	if (InteractibleSelected)
	{
		bool hide = IInteractInterface::Execute_GetInteractionType(InteractibleSelected) == EInteractibleType::EOneClick;
		if (!myController) {
			myController = Cast<AMyPlayerController>(GetController());
		}
		IInteractInterface::Execute_Interact(InteractibleSelected, myController);
		WidgetInteract->SetVisibility(hide , true);
		InteractibleSelected = !hide ? nullptr : InteractibleSelected;
		
	}
}
#pragma endregion


#pragma region Input Handling

void APlayerCharacter::TopFaceButtonAction()
{

}
void APlayerCharacter::BottomFaceButtonAction()
{
	Interact();
}
void APlayerCharacter::LeftFaceButtonAction()
{
	if (bHasLantern) 
	{
		myLantern->ToggleLight(!bLanternOut);
		bLanternOut = !bLanternOut;
		PlayLanternAnim();
		FAttachmentTransformRules AttachRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, false);
		myLantern->AttachToComponent(GetMesh(), AttachRules, bLanternOut? SocketHandLantern : SocketBackLantern);
	}
	
}
void APlayerCharacter::RightFaceButtonAction()
{
}
void APlayerCharacter::SetCanUseLantern(bool bCanUse)
{
	bHasLantern = bCanUse;
	if (bHasLantern) {
		CreateLantern();
	}
	else {
		if (myLantern!=nullptr) {
			myLantern->Destroy();
		}
		
	}
}
void APlayerCharacter::CreateLantern()
{
	if (myLantern) return;
	UWorld* const World = GetWorld();
	if (World) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		SpawnParams.Owner = this;
		
		myLantern = World->SpawnActor<APlayerLantern>(LanternClass,SpawnParams);
		if (myLantern) {
			FAttachmentTransformRules AttachRules = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative,false);
			myLantern->AttachToComponent(GetMesh(), AttachRules, SocketBackLantern);
			//myLantern->ToggleLight(false);
		}
	}
}
void APlayerCharacter::SetCanIdle(bool bNewVal)
{
	bCanIdle = bNewVal;
}
#pragma endregion




#pragma region Character Movement

	//Man, do I HATE doing things on tick, sadly there really wasnt a good way to do this, axis bindings are also like ticks when their value is 0, not kidding, try it out
	//So with that in mind, screw it, its on tick, it aint a very heavy thing

void APlayerCharacter::MoveForward(float Value)
{


	if ((Controller != nullptr) && (Value != 0.0f))
	{
		FVector Direction;
		if (myController && myController->GetCurrentCamera()) 
		{
			Direction = FRotationMatrix(myController->GetCurrentCamera()->GetComponentRotation()).GetUnitAxis(EAxis::X);
			FVector UpVect = GetActorUpVector();
			Direction =UKismetMathLibrary::ProjectVectorOnToPlane(Direction, UpVect);
			Direction.Normalize();		
		}
		else
		{
			Direction = GetController()->GetViewTarget()->GetActorForwardVector();
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		}
		AddMovementInput(Direction, Value);
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange, FString::Printf(TEXT("Forward Vector %s"), *Direction.ToString()));
		}
	}
}

void APlayerCharacter::MoveRight(float Value)
{

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		FVector Direction;
		if (myController && myController->GetCurrentCamera()) {
			Direction = FRotationMatrix(myController->GetCurrentCamera()->GetComponentRotation()).GetUnitAxis(EAxis::Y);
			FVector UpVect = GetActorUpVector();
			Direction = UKismetMathLibrary::ProjectVectorOnToPlane(Direction, UpVect);
		}
		else
		{
			Direction = GetController()->GetViewTarget()->GetActorRightVector();
			const FRotator Rotation = Controller->GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		}	
		AddMovementInput(Direction, Value);
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(-1, GetWorld()->DeltaTimeSeconds, FColor::Orange, FString::Printf(TEXT("Right Vector %s"), *Direction.ToString()));
		}
	}
}
#pragma endregion


