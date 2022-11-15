// Fropple Studios Copyright


#include "Interaction/InspectableItem.h"
#include "MyComponents/InspectionCollisionComponent.h"

// Sets default values
AInspectableItem::AInspectableItem()
{
	PrimaryActorTick.bCanEverTick = false;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ItemMesh"));
	RootComponent = ItemMesh;

	InteractCollision = CreateDefaultSubobject<UInspectionCollisionComponent>(FName("Inspection 1"));
	InteractCollision->SetupAttachment(RootComponent);
}

void AInspectableItem::SetStudio(AItemInspectionStudio* studio)
{
	MyStudio = studio;
}

// Called when the game starts or when spawned
void AInspectableItem::BeginPlay()
{
	Super::BeginPlay();
}


