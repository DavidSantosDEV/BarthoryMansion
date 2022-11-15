// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "UserGameConfig.generated.h"

/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API UUserGameConfig : public USaveGame
{
	GENERATED_BODY()
		/*
		- Icons de Gamepad
		- Volume
		- Show tutorial
		*/
public:
	//UPROPERTY(Config)
	bool bShowTutorials;
	//UPROPERTY(Config)
	float fVolumeMain;
	//UPROPERTY(Config)
	float fVolumeSFX;
	//UPROPERTY(Config)
	float fVolumeMusic;
	//UPROPERTY(Config,meta = (ClampMin = 0, ClampMax = 2))
	int IconIndex;
};
