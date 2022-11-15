// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthEvent, AActor*, DamageCauser);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BARTHORYMANSION_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float fMaxHealth = 100;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Health")
	float fCurrentHealth;

	bool bIsAlive = true;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:	

	UFUNCTION(BlueprintCallable, Category="Health")
	void Heal(float ammount);

	FORCEINLINE bool IsAlive() const { return bIsAlive; }

	UPROPERTY(BlueprintAssignable, Category = "EventDispatchers")
	FOnHealthEvent OnActorDied;
};
