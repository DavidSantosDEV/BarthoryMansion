// Fropple Studios Copyright


#include "Characters/CustomPlantNPC.h"
#include "Kismet/GameplayStatics.h"

void ACustomPlantNPC::ExtendInteract()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	if (Player) {
		Player->SetActorHiddenInGame(true);
	}
}

void ACustomPlantNPC::ExtendStopInteract()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player) {
		Player->SetActorHiddenInGame(false);
	}
}
