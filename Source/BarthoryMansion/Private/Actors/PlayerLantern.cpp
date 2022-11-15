// Fropple Studios Copyright


#include "Actors/PlayerLantern.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PointLightComponent.h"

// Sets default values
APlayerLantern::APlayerLantern()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Lantern Root"));
	RootComponent = MyRoot;


	MyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lantern Mesh"));
	MyMeshComponent->SetSimulatePhysics(true);
	MyMeshComponent->SetupAttachment(RootComponent);

	MyLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("Lantern Light"));
	MyLight->SetupAttachment(MyMeshComponent);
	MyLight->SetVisibility(false);
}

void APlayerLantern::ToggleLight(bool bNewValue)
{
	MyLight->SetVisibility(bNewValue, true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),bNewValue ? SoundOn : SoundOff , GetActorLocation());
	OnLightToggled(bNewValue);
}


