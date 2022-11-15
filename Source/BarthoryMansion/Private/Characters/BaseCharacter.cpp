// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BaseCharacter.h"
#include "MyComponents/HealthComponent.h"
#include "MYComponents/IKFootTracingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(FName("HealthComponent"));
	HealthComponent->OnActorDied.AddDynamic(this, &ABaseCharacter::OnCharacterDie);
}

void ABaseCharacter::SetRagdollPhysics()
{
	USkeletalMeshComponent* CharMesh = GetMesh();

	if (!IsPendingKill() && (CharMesh && CharMesh->GetPhysicsAsset()))
	{
		CharMesh->SetAllBodiesSimulatePhysics(true);
		CharMesh->SetSimulatePhysics(true);
		CharMesh->WakeAllRigidBodies();
		CharMesh->bBlendPhysics = true;
	}
	UCharacterMovementComponent* CharMovementComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharMovementComp)
	{
		CharMovementComp->StopMovementImmediately();
		CharMovementComp->DisableMovement();
		CharMovementComp->SetComponentTickEnabled(false);
	}
}
void ABaseCharacter::OnCharacterDie(AActor* DamageCauser)
{
	SetRagdollPhysics();
}

