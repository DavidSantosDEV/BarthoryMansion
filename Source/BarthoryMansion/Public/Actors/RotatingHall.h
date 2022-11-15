// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RotatingHall.generated.h"

UENUM(BlueprintType)
enum EPositionAxis {
	EY,
	EX,
	EZ,
};

UCLASS()
class BARTHORYMANSION_API ARotatingHall : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Positioning", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* MyRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Positioning", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* HallwayCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Positioning", meta = (AllowPrivateAccess = "true"))
	class UBillboardComponent* PointStart;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Positioning", meta = (AllowPrivateAccess = "true"))
	UBillboardComponent* PointEnd;
public:	
	// Sets default values for this actor's properties
	ARotatingHall();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Positioning")
	AActor* HallwayMesh;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Positioning")
	TEnumAsByte<EPositionAxis> Axis = EPositionAxis::EY;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Positioning")
	FRotator RotationStart;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Positioning")
	FRotator RotationEnd;

	AActor* PlayerActor;

protected:
	UFUNCTION()
	void OnActorEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnActorLeave(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void RotateHallway(FVector positionStart, FVector positionEnd, FVector PlayerPosition);
	
	FRotator CalculateRotation(FRotator rotPointZero, FRotator rotPointEnd, float fAlpha);

public:	
	virtual void Tick(float DeltaTime) override;

};
