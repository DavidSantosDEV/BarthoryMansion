// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SplineFollowCamera.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCameraToggled, bool, IsActiveNow);

UENUM(BlueprintType)
enum ECameraRotationType {
	ENone,
	EFollowPlayer,
	EFollowSpline,
};

UCLASS()
class BARTHORYMANSION_API ASplineFollowCamera : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = CameraSpline, meta = (AllowPrivateAccess = "true"))
	class UBillboardComponent* BillboardIcon;

	UPROPERTY(VisibleAnywhere, Category = CameraSpline, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraView;

	UPROPERTY(VisibleAnywhere, Category = CameraSpline, meta = (AllowPrivateAccess = "true"))
	class USplineComponent* SplineCamera;

	UPROPERTY(VisibleAnywhere, Category = CameraSpline, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* BoundsBox;
protected:
	UPROPERTY(BlueprintReadOnly, Category = CameraSpline)
	class APlayerCharacter* Player;

	UPROPERTY(EditInstanceOnly,BlueprintReadOnly, Category = "A CameraSplineRotation")
	TEnumAsByte<ECameraRotationType> RotationType;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineRotation") //Pitch
	bool bRotateY;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineRotation") //Yaw
	bool bRotateZ;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineRotation", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float fRotationInterpSpeed;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineRotation")
	FRotator RotationOffset;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineMovement")
	FVector CameraOffset;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineMovement", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float fInterpTime;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineMovement")
	bool bCameraMoveAlongSpline=true;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineBlend")
	float fBlendExp;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineBlend")
	float fBlendTime;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineBlend")
	TEnumAsByte<EViewTargetBlendFunction> BlendFunc;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineBlend")
	bool bForceBlendTime=false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineBlend")
	float fBlendTimeOut;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineBlend")
	TEnumAsByte<EViewTargetBlendFunction> BlendFuncOut;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "A CameraSplineBlend")
	float fBlendExpOut;
public:
	UPROPERTY(BlueprintAssignable, Category = "SplineCamera")
	FOnCameraToggled OnCameraToggledDelegate;
public:	
	// Sets default values for this actor's properties
	ASplineFollowCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SetCameraLocation();
	UFUNCTION()
	void SetCameraRotation();

	UFUNCTION()
	void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when this actor becomes the given PlayerController's ViewTarget. Triggers the Blueprint event K2_OnBecomeViewTarget. */
	virtual void BecomeViewTarget(class APlayerController* PC) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = CameraSpline)
	void ToggleCamera(bool bActive);

	UFUNCTION(BlueprintPure, Category = "Spline Camera")
	UCameraComponent* GetCameraComponent() { return CameraView; };

	bool GetForcelendsOut() { return bForceBlendTime; }

	float GetBlendTimeOut() { return fBlendTimeOut; }
	TEnumAsByte<EViewTargetBlendFunction> GetBlendFuncOut() { return BlendFuncOut; }
	float GetBlendExpOut() { return fBlendExpOut; }

};
