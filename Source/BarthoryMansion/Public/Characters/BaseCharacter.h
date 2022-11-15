// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class BARTHORYMANSION_API ABaseCharacter : public ACharacter/*, public UInteractible*/
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;
public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

	void SetRagdollPhysics();

	UFUNCTION()
	virtual void OnCharacterDie(AActor* DamageCauser);
};
