// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IKFootTracingComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BARTHORYMANSION_API UIKFootTracingComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	class ACharacter* Character;
	UPROPERTY()
	class UIKAnimInstance* AnimInstance;

public:
	UPROPERTY(EditDefaultsOnly, Category = "IK")
	FName RightFootBoneName = "ik_foot_r";
	UPROPERTY(EditDefaultsOnly, Category = "IK")
	FName LeftFootBoneName = "ik_foot_l";
	UPROPERTY(EditDefaultsOnly, Category = "IK")
	FName RootBoneName = "root";

	UPROPERTY()
	TArray<AActor*> IgnoreActors;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	float Displacement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	float IKInterpSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	FRotator RRot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	FRotator LRot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	float RIK;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "IK", meta = (AllowPrivateAccess = "true"))
	float LIK;

public:	
	// Sets default values for this component's properties
	UIKFootTracingComponent();
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void FootIK(float DeltaTime);
	TTuple<bool, float> CapsuleDistance(FName SocketName, ACharacter* Char);
	TTuple<bool, float, FVector> FootLineTrace(FName SocketName, ACharacter* Char);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
