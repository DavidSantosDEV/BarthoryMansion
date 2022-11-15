// Fropple Studios Copyright

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FMODBlueprintStatics.h"
#include "MansionGameMode.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMusicChanged);
/**
 * 
 */
UCLASS()
class BARTHORYMANSION_API AMansionGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Music")
	TMap<UFMODEvent*, int> EventsAndTime;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	UFMODEvent* CurrentEvent = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	FFMODEventInstance CurrentPlayingMusic = FFMODEventInstance();

	UPROPERTY(BlueprintReadOnly, Category = "Music")
	FFMODEventInstance PreviousPlayingMusic= FFMODEventInstance();

public:

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Music")
	int GetTimeForEvent(UFMODEvent* EventIn);

	UFUNCTION(BlueprintCallable, Category = "Music")
	void SaveMusicTime(const FFMODEventInstance EventMusic);

	UFUNCTION(BlueprintCallable, Category = "Music")
	void ChangeMusic(class UFMODEvent* MusicEvent);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Music")
	void LowerPreviousMusic();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Music")
	void LowerCurrentMusic();
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Music")
	void IncreaseCurrentMusic();

	UFUNCTION(BlueprintCallable)
		void StopCurrentMusic();

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Music")
	UFMODEvent* GetCurrentMusic();

	UPROPERTY(BlueprintAssignable, Category = "Music")
	FOnMusicChanged MusicChanged;
};
