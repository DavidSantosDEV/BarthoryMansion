// Fill out your copyright notice in the Description page of Project Settings.


#include "MyComponents/HealthComponent.h"
#include "GenericTeamAgentInterface.h" 

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	fCurrentHealth = fMaxHealth;
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::HandleTakeDamage);
}

void UHealthComponent::HandleTakeDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	fCurrentHealth = FMath::Clamp(fCurrentHealth - Damage, 0.f, fMaxHealth);
	if (fCurrentHealth == 0) {
		bIsAlive = false;
		OnActorDied.Broadcast(DamageCauser);
	}
}

void UHealthComponent::Heal(float ammount)
{
	fCurrentHealth = FMath::Clamp(fCurrentHealth + ammount, 0.f, fMaxHealth);
}

