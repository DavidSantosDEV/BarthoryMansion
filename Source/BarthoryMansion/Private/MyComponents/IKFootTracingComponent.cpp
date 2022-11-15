// Fropple Studios Copyright


#include "MyComponents/IKFootTracingComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h" 
#include "Kismet/KismetMathLibrary.h"
#include "IKAnimInstance.h"

// Sets default values for this component's properties
UIKFootTracingComponent::UIKFootTracingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UIKFootTracingComponent::BeginPlay()
{
	Super::BeginPlay();
	Character = Cast<ACharacter>(GetOwner());
	if (Character) {
		IgnoreActors.Add(Character);
		AnimInstance = Cast<UIKAnimInstance>(Character->GetMesh()->GetAnimInstance());
	}

	if (!AnimInstance || !Character) {
		SetComponentTickEnabled(false);
	}

}

void UIKFootTracingComponent::FootIK(float DeltaTime)
{
	if (Character && !Character->GetCharacterMovement()->IsFalling()) // My man cant be falling now can he? I disabled his jump anyways
	{
		TTuple<bool, float> Foot_R = CapsuleDistance(RightFootBoneName, Character);
		TTuple<bool, float> Foot_L = CapsuleDistance(LeftFootBoneName, Character);

		if (Foot_L.Get<0>() || Foot_R.Get<0>())
		{
			const float Selectfloat = UKismetMathLibrary::SelectFloat(Foot_L.Get<1>(), Foot_R.Get<1>(), Foot_L.Get<1>() >= Foot_R.Get<1>());
			Displacement = FMath::FInterpTo(Displacement, (Selectfloat - 98.f) * -1.f, DeltaTime, IKInterpSpeed);

			TTuple<bool, float, FVector> FootTrace_R = FootLineTrace(RightFootBoneName, Character);
			TTuple<bool, float, FVector> FootTrace_L = FootLineTrace(LeftFootBoneName, Character);

			const float Distance_R = FootTrace_R.Get<1>();
			const FVector FootRVector(FootTrace_R.Get<2>());
			const FRotator MakeRRot(UKismetMathLibrary::DegAtan2(FootRVector.X, FootRVector.Z) * -1.f, 0.f, UKismetMathLibrary::DegAtan2(FootRVector.Y, FootRVector.Z));

			RRot = FMath::RInterpTo(RRot, MakeRRot, DeltaTime, IKInterpSpeed);
			RIK = FMath::FInterpTo(RIK, (Distance_R - 110.f) / -45.f, DeltaTime, IKInterpSpeed);

			const float Distance_L = FootTrace_L.Get<1>();
			const FVector FootLVector(FootTrace_L.Get<2>());
			const FRotator MakeLRot(UKismetMathLibrary::DegAtan2(FootLVector.X, FootLVector.Z) * -1.f, 0.f, UKismetMathLibrary::DegAtan2(FootLVector.Y, FootLVector.Z));

			LRot = FMath::RInterpTo(LRot, MakeLRot, DeltaTime, IKInterpSpeed);
			LIK = FMath::FInterpTo(LIK, (Distance_L - 110.f) / -45.f, DeltaTime, IKInterpSpeed);
		}
	}
	else
	{
		LRot = FMath::RInterpTo(LRot, FRotator::ZeroRotator, DeltaTime, IKInterpSpeed);
		LIK = FMath::FInterpTo(LIK, 0.f, DeltaTime, IKInterpSpeed);

		RRot = FMath::RInterpTo(RRot, FRotator::ZeroRotator, DeltaTime, IKInterpSpeed);
		RIK = FMath::FInterpTo(RIK, 0.f, DeltaTime, IKInterpSpeed);
	}
	AnimInstance->SetLeftFootIK(LIK);
	AnimInstance->SetRightFootIK(RIK);
	AnimInstance->SetDisplacement(Displacement);
	AnimInstance->SetRightFootRot(RRot);
	AnimInstance->SetLeftFootRot(LRot);
}

TTuple<bool, float> UIKFootTracingComponent::CapsuleDistance(FName SocketName, ACharacter* Char)
{
	const FVector WorldLocation{ Char->GetMesh()->GetComponentLocation() };
	const FVector BreakVector{ WorldLocation + FVector(0.f,0.f,98.f) };

	const FVector SocketLocation{ Char->GetMesh()->GetSocketLocation(SocketName) };

	const FVector Start{ SocketLocation.X,SocketLocation.Y,BreakVector.Z };
	const FVector End{ Start - FVector(0.f,0.f,151.f) };

	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start,
		End,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForOneFrame,
		HitResult,
		false, FLinearColor::Yellow);

	const bool Result(HitResult.bBlockingHit);

	return MakeTuple(Result, HitResult.Distance);
}

TTuple<bool, float, FVector> UIKFootTracingComponent::FootLineTrace(FName SocketName, ACharacter* Char)
{
	
	const FVector SocketLocation{ Char->GetMesh()->GetSocketLocation(SocketName) };
	const FVector RootLocation(Char->GetMesh()->GetSocketLocation(RootBoneName));

	const FVector Start{ SocketLocation.X,SocketLocation.Y,RootLocation.Z };

	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingle(
		this,
		Start + FVector(0.f, 0.f, 105.f),
		Start + FVector(0.f, 0.f, -105.f),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		IgnoreActors,
		EDrawDebugTrace::None,
		HitResult,
		false);

	const bool Result(HitResult.bBlockingHit);

	if (HitResult.bBlockingHit)
	{
		return MakeTuple(Result, HitResult.Distance, HitResult.Normal);
	}
	else
	{
		return MakeTuple(Result, 999.f, FVector::ZeroVector);
	}
}

// Called every frame
void UIKFootTracingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FootIK(DeltaTime);
}

