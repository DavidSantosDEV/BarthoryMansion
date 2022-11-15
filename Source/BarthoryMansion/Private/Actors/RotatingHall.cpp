// Fropple Studios Copyright


#include "Actors/RotatingHall.h"
#include "Components/BillboardComponent.h" 
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h" 
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARotatingHall::ARotatingHall()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Axis = EPositionAxis::EY;

	MyRoot = CreateDefaultSubobject<USceneComponent>("ActorRoot");
	RootComponent = MyRoot;

	PointStart = CreateDefaultSubobject<UBillboardComponent>("Point Start");
	PointStart->SetupAttachment(RootComponent);

	PointEnd = CreateDefaultSubobject<UBillboardComponent>("Point End");
	PointEnd->SetupAttachment(RootComponent);

	HallwayCollision = CreateDefaultSubobject<UBoxComponent>("HallwayCollision");
	HallwayCollision->SetupAttachment(RootComponent);
	//HallwayCollision->SetBoxExtent(HallwayMesh->GetStaticMesh()->GetBounds());
}

// Called when the game starts or when spawned
void ARotatingHall::BeginPlay()
{
	Super::BeginPlay();

	HallwayCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ARotatingHall::OnActorEnter);
	HallwayCollision->OnComponentEndOverlap.AddUniqueDynamic(this, &ARotatingHall::OnActorLeave);
	if (HallwayMesh) {
		HallwayMesh->SetActorRotation(RotationStart);
		HallwayMesh->SetActorRotation(CalculateRotation(RotationStart, RotationEnd, 1));
	}
	SetActorTickEnabled(false); //Default to no tick
}

void ARotatingHall::OnActorEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(Cast<APlayerCharacter>(OtherActor)) 
	{
		PlayerActor = OtherActor;
		SetActorTickEnabled(true);
	}
}

void ARotatingHall::OnActorLeave(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<APlayerCharacter>(OtherActor)) {
		
		SetActorTickEnabled(false);
		PlayerActor = nullptr;
	}
}

void ARotatingHall::RotateHallway(FVector positionStart, FVector positionEnd, FVector PlayerPosition)
{
	/*
	S = 450
	F = 500
	p = Clamp(470, S,F)
	((F-S)- (F-P)) / (F-S) = %
	*/
	
	float EndValue;
	float playerValue;
	float distanceStartEnd;
	UE_LOG(LogTemp, Warning, TEXT("Start %s"), *positionStart.ToString())
	UE_LOG(LogTemp, Warning, TEXT("End %s"), *positionEnd.ToString())
	UE_LOG(LogTemp, Warning, TEXT("Player %s"), *PlayerPosition.ToString())

	switch (Axis)
	{
	default:
	case EPositionAxis::EX:
		EndValue = FMath::Abs(positionEnd.X);
		distanceStartEnd = FMath::Abs(positionEnd.X) - FMath::Abs(positionStart.X);
		playerValue = FMath::Clamp(FMath::Abs(PlayerPosition.X), FMath::Abs(positionStart.X), FMath::Abs(positionEnd.X));
		break;
	case EPositionAxis::EY:
		EndValue = FMath::Abs(positionEnd.Y);
		distanceStartEnd = FMath::Abs(positionEnd.Y) - FMath::Abs(positionStart.Y);
		playerValue = FMath::Clamp(FMath::Abs(PlayerPosition.Y), FMath::Abs(positionStart.Y), FMath::Abs(positionEnd.Y));
		break;
	case EPositionAxis::EZ:
		EndValue = FMath::Abs(positionEnd.Z);
		distanceStartEnd = FMath::Abs(positionEnd.Z) - FMath::Abs(positionStart.Z);
		playerValue = FMath::Clamp(FMath::Abs(PlayerPosition.Z), FMath::Abs(positionStart.Z), FMath::Abs(positionEnd.Z));
		break;
	}
	/*
	float distanceStartEnd = positionEnd.Y - positionStart.Y;
	float playerY = FMath::Clamp(PlayerPosition.Y, positionStart.Y, positionEnd.Y);
	*/
	//float fPositionAlpha = (distanceStartEnd - (positionEnd.Y-playerY) )/distanceStartEnd;

	float endValCalc = EndValue - playerValue;
	float fPositionAlpha = (distanceStartEnd - endValCalc) / distanceStartEnd;
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Red, FString::Printf(TEXT("Distance Y %f"), fPositionAlpha));
	}
	HallwayMesh->SetActorRotation(CalculateRotation(RotationStart, RotationEnd, fPositionAlpha));
}

FRotator ARotatingHall::CalculateRotation(FRotator rotPointZero, FRotator rotPointEnd, float fAlpha)
{
	const float Value = FMath::Clamp(fAlpha, 0.f, 1.f);
	// if DeltaTime is 0, do not perform any interpolation
	if (Value == 0.f)
	{
		return rotPointZero;
	}
	else if (Value == 1.f) {
		return rotPointEnd;
	}

	//alpha * end + (1 - alpha) * start 

	FRotator rot = Value * rotPointEnd + (1 - Value) * rotPointZero;

	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1,GetWorld()->GetDeltaSeconds(), FColor::Blue, FString::Printf(TEXT("Rot X: %f Y: %f Z:%f"), rot.Pitch, rot.Yaw, rot.Roll));
	}

	return rot;
}

// Called every frame
void ARotatingHall::Tick(float DeltaTime) //Reason I use tick is cause its more responsive, yes its crap, yes it sucks, but yes its needed
{
	Super::Tick(DeltaTime);
	if (PlayerActor && HallwayMesh) {
		RotateHallway(PointStart->GetComponentLocation(), PointEnd->GetComponentLocation(), PlayerActor->GetActorLocation());	
	}
	
}

